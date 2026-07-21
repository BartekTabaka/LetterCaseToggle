#include "App.h"

#include "SystemHook.h"
#include "Core/TextTransform.h"
#include <string>
#include <QEventLoop>
#include <QLabel>
#include <QTimer>

App* g_App = nullptr;

namespace
{
	// Waits `ms` milliseconds WITHOUT blocking the Qt/Windows message pump.
	// A plain Sleep() stalls the very thread that delivers the low-level
	// keyboard hook's callbacks (WH_KEYBOARD_LL delivery requires the
	// installing thread to be pumping messages). Real key events that occur
	// during a Sleep() aren't lost, but they get buffered by Windows and only
	// handed to KeyboardProc in a rapid-fire burst once the thread resumes
	// pumping - which is what made the app feel like it needed the key held
	// down rather than tapped.
	void WaitWithEvents(int ms)
	{
		QEventLoop loop;

		QTimer::singleShot(ms, &loop, &QEventLoop::quit);

		loop.exec();
	}

	// Polls the clipboard sequence number instead of guessing a fixed delay -
	// returns as soon as the clipboard actually changes (usually well under
	// the old fixed waits), or after timeoutMs if it never does.
	bool WaitForClipboardChange(DWORD previousSeq, int timeoutMs)
	{
		const int pollMs = 10;
		int waited = 0;

		while (waited < timeoutMs) {
			if (GetClipboardSequenceNumber() != previousSeq)
				return true;

			WaitWithEvents(pollMs);
			waited += pollMs;
		}
		return GetClipboardSequenceNumber() != previousSeq;
	}
}

App::App(QApplication& app) : m_App(app)
{
	// Tray (uninitialized!)

	// Clipboard
	m_Clipboard = QApplication::clipboard();
}

// Toggles the case of the currently selected text by copying it to the clipboard,
// transforming it, and pasting it back, while preserving the previous clipboard content.
// If Caps Lock is pressed but no text is selected (or no valid target window can be
// found), we fall back to a normal simulated Caps Lock press via SendCapsLockToggle()
// so typing in caps still works as usual.
void App::HandleCaps()
{
	if (m_Busy) 
		return;
	m_Busy = true;
	qDebug() << "HandleCaps()";

	const HWND target = GetTargetWindow();
	const bool targetActive = IsTargetWindowActive(target);
	if (!target) {
		// Nothing to toggle-case and nowhere to send it - let Caps Lock behave
		// normally so the user can still type in caps without reaching for Shift.
		SendCapsLockToggle();
		m_Busy = false;
		return;
	}

	QString previous = m_Clipboard->text();
	const DWORD seqBeforeClear = GetClipboardSequenceNumber();
	m_Clipboard->clear();
	WaitWithEvents(30);

	CopySelectionFromTarget(target);
	WaitForClipboardChange(seqBeforeClear, 80);

	QString selected = m_Clipboard->text();

	if (selected.isEmpty()) {
		if (!targetActive)
			FocusTargetWindow(target);
		WaitWithEvents(30);

		// Safety check
		if (!IsOwnProcessWindow(GetForegroundWindow())) {
			const DWORD seqBeforeSendC = GetClipboardSequenceNumber();
			SendCtrlCommand('C');
			WaitForClipboardChange(seqBeforeSendC, 150);
			selected = m_Clipboard->text();
		}
	}

	if (selected.isEmpty()) {
		SendCapsLockToggle();
		m_Clipboard->setText(previous);
		m_Busy = false;
		return;
	}

	qDebug() << "Text was selected";
	std::wstring input = selected.toStdWString();
	std::wstring toggled = Core::ToggleCase(input);

	m_Clipboard->setText(QString::fromStdWString(toggled));
	WaitWithEvents(30);

	if (!targetActive)
		FocusTargetWindow(target);
	WaitWithEvents(30);

	// Safety check
	if (!IsOwnProcessWindow(GetForegroundWindow())) {
		SendCtrlCommand('V');
		WaitWithEvents(120);
	}

	qDebug() << "Escaping HandleCaps()";
	m_Clipboard->setText(previous);
	m_Busy = false;
}

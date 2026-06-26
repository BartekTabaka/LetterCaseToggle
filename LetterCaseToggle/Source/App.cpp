#include "App.h"

#include "SystemHook.h"
#include "Core/TextTransform.h"
#include <string>
#include <QLabel>

App* g_App = nullptr;

App::App(QApplication& app) : m_App(app)
{
	// Tray

	// Clipboard
	m_Clipboard = QApplication::clipboard();
}

/// <summary>
///	Toggles the case of the currently selected text by copying it to the clipboard,
/// transforming it, and pasting it back, while preserving the previous clipboard content.
/// If CapsLock has been pressed but no text is selected, the original clipboard content is restored.
/// Includes short delays to ensure clipboard and input events are processed correctly by the system.
/// </summary>
void App::HandleCaps()
{
	if (m_Busy) return;
	m_Busy = true;

	QString previous = m_Clipboard->text();
	m_Clipboard->clear();

	SendCtrlCommand('C');
	Sleep(80);

	QString selected = m_Clipboard->text();
	if (!selected.isEmpty()) {
		std::wstring input = selected.toStdWString();
		std::wstring toggled = Core::ToggleCase(input);

		m_Clipboard->setText(QString::fromStdWString(toggled));

		Sleep(30);
		SendCtrlCommand('V');
		Sleep(80);
	}

	m_Clipboard->setText(previous);
	m_Busy = false;
}

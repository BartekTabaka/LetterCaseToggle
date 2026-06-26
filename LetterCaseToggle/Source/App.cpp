#include "App.h"

#include "SystemHook.h"
#include "Core/TextTransform.h"
#include <string>

App::App(QApplication& app) : m_App(app)
{
	// Tray

	// Clipboard
	m_Clipboard = QApplication::clipboard();
}

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

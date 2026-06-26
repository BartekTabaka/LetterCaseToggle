#pragma once
#include <QApplication>
#include <QSystemTrayIcon>
#include <QClipboard>

class App;
static App* g_App = nullptr;

class App : public QObject {
	Q_OBJECT
public:
	explicit App(QApplication& app);

	void HandleCaps();
private:
	QApplication& m_App;
	QSystemTrayIcon* m_Tray;
	QClipboard* m_Clipboard;
	bool m_Busy = false;
};

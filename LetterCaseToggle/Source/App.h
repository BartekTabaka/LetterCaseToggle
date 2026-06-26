#pragma once
#include <QApplication>
#include <QSystemTrayIcon>
#include <QClipboard>

// ─── Forward ─────────────────────────────
class App;
static App* g_App = nullptr; // Pointer to the application instance for global access 

// ─── App class ───────────────────────────
class App : public QObject {
	Q_OBJECT
public:
	explicit App(QApplication& app);

	// Handles CapsLock key press event
	void HandleCaps();
private:
	QApplication& m_App;		// Qt app instance used for event dispatching
	QSystemTrayIcon* m_Tray;	// Application tray icon for running in background
	QClipboard* m_Clipboard;	// Access to system clipboard
	bool m_Busy = false;		// Prevents re-entry while clipboard operation is running
};

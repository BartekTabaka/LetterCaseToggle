#pragma once
#include <QApplication>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

#include <Core/TimingProfile.h>

// ─── Forward ─────────────────────────────
class App;
extern App *g_App; // Pointer to the application instance for global access 

// ─── App class ───────────────────────────
class App : public QObject {
	Q_OBJECT
public:
	explicit App(QApplication& app);
	~App();

	// Handles CapsLock key press event
	void HandleCaps();
private:
	void TrayClicked(QSystemTrayIcon::ActivationReason reason);
	void SetSpeed(Core::Speed speed);
private:
	QApplication& m_App;		// Qt app instance used for event dispatching

	QSystemTrayIcon *m_Tray;	// Application tray icon for running in background
	QMenu *m_TrayMenu;			// Menu used by app's tray icon
	QAction *m_QuitAction;		

	QClipboard *m_Clipboard;	// Access to system clipboard
	bool m_Busy = false;		// Prevents re-entry while clipboard operation is running

	Core::TimingProfile m_Timing = Core::GetTimingProfile(Core::Speed::Normal);
	QMenu *m_SpeedMenu;
	QActionGroup *m_SpeedGroup;
	Core::Speed m_CurrentSpeed = Core::Speed::Normal;
};

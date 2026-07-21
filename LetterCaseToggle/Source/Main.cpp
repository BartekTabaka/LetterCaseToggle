#include "App.h"
#include "SystemHook.h"
#include "Core/TextTransform.h"
#include <Windows.h>

// This app sends synthetic Ctrl+C to whatever window has focus when Caps
// Lock is pressed with nothing selected elsewhere. App.cpp already guards
// against sending that into our own process, but as a hard backstop, make
// sure a Ctrl+C landing on OUR OWN console (real or synthetic, from any
// remaining edge case) can never kill this process outright. Ctrl+Break and
// window-close/logoff/shutdown still work normally.
BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType)
{
    if (ctrlType == CTRL_C_EVENT)
        return TRUE; // handled - swallow it, don't run the default action (process termination)

    return FALSE; // let Windows handle everything else (close, logoff, shutdown) normally
}

int main(int argc, char *argv[])
{
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    // App definition
    QApplication app(argc, argv);
    App logic(app);

    g_App = &logic;
    qDebug() << &g_App;

    KeyboardHook kh;
    g_KeyboardHook = &kh;
    if (!kh.Start())
        return 1;

    // Unhook before closing the application
    QObject::connect(&app, &QApplication::aboutToQuit, [&kh] {
        qDebug() << "kh.stop";
        kh.Stop();
    });

    return app.exec();
}

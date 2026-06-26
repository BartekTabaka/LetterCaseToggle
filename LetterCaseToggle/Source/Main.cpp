#include "App.h"
#include "SystemHook.h"
#include "Core/TextTransform.h"

int main(int argc, char *argv[])
{
    // App definition
    QApplication app(argc, argv);
    App logic(app);

    g_App = &logic;

    KeyboardHook kh;
    if (!kh.Start())
        return 1;

    // Unhook before closing the application
    QObject::connect(&app, &QApplication::aboutToQuit, [&kh] {
        kh.Stop();
    });

    return app.exec();
}

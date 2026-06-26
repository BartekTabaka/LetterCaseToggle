#include <print>
#include <QApplication>

#include "App.h"
#include "Core/TextTransform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    App logic(app);

    g_App = &logic;

    return app.exec();
}

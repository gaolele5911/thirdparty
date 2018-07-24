#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "launcherservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    LauncherService::init(engine);
    QString strQmlContent = "import SSPLink 1.0; Launcher { }";
    engine.loadData(strQmlContent.toLatin1());

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}

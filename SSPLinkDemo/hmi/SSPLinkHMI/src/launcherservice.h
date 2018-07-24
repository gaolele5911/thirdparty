#ifndef LAUNCHER_SERVICE_H
#define LAUNCHER_SERVICE_H

#include <QObject>

#define REGISTER_LAUNCHER_SERVICE( engine ) \
    engine.addImportPath(":/SSPLinkQml/"); \
    qmlRegisterType<LauncherService>("SSPLink", 1, 0,"LauncherService"); \

class QQmlApplicationEngine;
class LauncherService: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LauncherService)

public:
    LauncherService();
    static void init(QQmlApplicationEngine &engine);

signals:

public slots:
    bool isArm();
    int add(int a,int b);
};

#endif // LAUNCHER_SERVICE_H

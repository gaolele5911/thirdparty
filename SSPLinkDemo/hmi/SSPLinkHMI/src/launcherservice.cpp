#include <qqml.h>
#include <QDebug>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include "launcherservice.h"
#include <string>
using namespace std;


LauncherService::LauncherService()
{
}

void LauncherService::init(QQmlApplicationEngine &engine)
{
    REGISTER_LAUNCHER_SERVICE(engine);
}

bool LauncherService::isArm()
{
#ifdef Q_PROCESSOR_ARM
    return true;
#endif
    return false;
}

int LauncherService::add(int a,int b)
{
    return a+b;
}

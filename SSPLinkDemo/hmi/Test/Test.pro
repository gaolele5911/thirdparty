QT += qml quick

CONFIG += c++11

TARGET = TestApp

TEMPLATE = app

DESTDIR = $$PWD/../../../../../dist/release/svp/lib/rn2_ext

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp

include($$PWD/../SSPLinkHMI/SSPLinkHMI.pri)

LIBS += -L$$PWD/../../../../../dist/release/svp/lib/rn2_ext -lSSPLinkHMI

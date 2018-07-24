#ifndef _SVP_NAVI_QML_H_
#define _SVP_NAVI_QML_H_

#include <QByteArray>

QByteArray SVP_DEMO_QML = "\
    import QtQuick 2.2; \
    import QtQuick.Window 2.2; \
    import QtQuick.Controls 1.2; \
    \
    Window { \
        id: root; \
        flags: Qt.CustomizeWindowHint | Qt.Popup; \
        width: 1280; \
        height: 720; \
        color: \"#55555555\"; \
        opacity: 2.0; \
        visible: true; \
        \
        Text { \
            width: 1280; \
            height: 100; \
            anchors.centerIn: parent; \
            anchors.verticalCenterOffset: -100; \
            anchors.horizontalCenterOffset: 0; \
            horizontalAlignment: Text.AlignHCenter; \
            text: \"Hello QML!\"; \
            font.pointSize: 48; \
            color: \"red\"; \
        }\
    } ";

QByteArray SVP_NAVI_QML = "import SSPLink 1.0; Launcher { }";

#endif /* _SVP_NAVI_QML_H_ */

import QtQuick 2.0
import QtQuick.Window 2.2
import SSPLink 1.0


Window {
    id: window
    visible: true
    width: 1920
    height: 720
    color: "#000000"

    Component.onCompleted: {
        console.log("Component.onCompleted");
    }

    LauncherService {
        id:launcherService

    }

    Text{
        id: text
        anchors.centerIn: parent
        text: "Hello SSPLink, Click to Call add function."
        color: "#FFFFFF"
        font.pixelSize: 68
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            text.text = launcherService.add(10,8);
        }
    }
}

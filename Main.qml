import QtQuick
import QtQuick.Controls

ApplicationWindow {
    minimumWidth: 640
    minimumHeight: 480

    width: 1200
    height: 800

    visible: true
    title: qsTr("SyroySync")

    Row {
        anchors.fill: parent

        StackView {
            id: stackView
            anchors.fill: parent
        }
    }
}

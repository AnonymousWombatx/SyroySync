import QtQuick
import QtQuick.Controls

ApplicationWindow {
    minimumWidth: 640
    minimumHeight: 480

    width: 1200
    height: 800

    visible: true
    title: qsTr("SyroySync")

    FontLoader {
        id: stdF
        source: Qt.resolvedUrl("resources/font/Quicksand.ttf")
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "intro.qml"
    }
}


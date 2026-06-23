import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    minimumWidth: 640
    minimumHeight: 480

    width: 1200
    height: 800

    visible: true
    title: qsTr("SyroySync")

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 20

            Button {
                text: "Downloads"
                onClicked: downloadDrawer.open()
                Layout.alignment: Qt.AlignCenter
            }
            Button {
                text: "← Back"
                enabled: stackView.depth > 1
                onClicked: stackView.pop()
                Layout.alignment: Qt.AlignCenter
            }
            Button {
                text: "⌂ Home"
                onClicked: stackView.replace(null, "IntroPage.qml")
                Layout.alignment: Qt.AlignCenter
            }
            Button {
                text: "Information"
                onClicked: stackView.push("Information.qml")
                Layout.alignment: Qt.AlignCenter
            }
        }
    }

    FontLoader {
        id: stdF
        source: Qt.resolvedUrl("resources/font/Quicksand.ttf")
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "IntroPage.qml"
    }



    DownloadDrawer {
        id: downloadDrawer

        height: parent.height
        width: parent.width*0.4
    }
}


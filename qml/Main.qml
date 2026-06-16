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

            Layout.alignment: Qt.AlignVCenter

            Button {
                text: "Downloads"
                onClicked: downloadDrawer.open()
            }
            Button {
                text: "← Back"
                visible: stackView.depth > 1
                onClicked: stackView.pop()
            }
            Button {
                text: "⌂ Home"
                onClicked: stackView.replace(null, "IntroPage.qml")
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


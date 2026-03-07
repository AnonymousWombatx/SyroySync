import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: load_panel

    property bool updateAvailable: false
    property bool checkFinished: false
    property string newVersion

    Component.onCompleted: {
        updater.checkForUpdates()
    }

    Connections {
        target: updater

        function onUpdateCheckFinished() {
            checkFinished = true
            updateAvailable = updater.updateAvailable
            newVersion = updater.latestVersion
        }
    }



    Image {
        source: Qt.resolvedUrl("resources/images/loading_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Rectangle {
        id: intro_rect
        width: parent.width*0.5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        border {
            id: intro_rect_border
            width: 2
            color: "khaki"
        }
        color: "aqua"
        opacity: 0.7
        radius: 20

        ColumnLayout {

            anchors.fill: parent
            anchors.margins: 20
            spacing: 40
            anchors.topMargin: 60
            anchors.bottomMargin: 60

            BusyIndicator {
                running: true
                Layout.preferredWidth: 75
                Layout.preferredHeight: 75
                Layout.alignment: Qt.AlignHCenter
                visible: !checkFinished
            }

            Text {
                text: !checkFinished
                    ? "Checking for updates ..."
                    : (updateAvailable
                       ?"Update available: Version " + newVersion
                        : "App up to date: Version " + newVersion
                       )
                font.family: stdF.name
                font.pixelSize: 32
                color: "black"

                Layout.alignment: Qt.AlignCenter
                width: parent.width
            }

            Button {
                //visible when check for update finished
                visible: checkFinished

                background: Rectangle {
                    color: "blue"
                    radius: 12
                }

                text: "Next"
                font.pixelSize: 48
                font.family: stdF.name
                palette.buttonText: "white"

                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: parent.width * 0.4
                Layout.preferredHeight: font.pixelSize * 1.5

                padding: 40

                onClicked: {
                    load_panel.StackView.view.push("LoadPage.qml")
                }



            }

        }

    }


}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: intro_panel

    Image {
        source: Qt.resolvedUrl("resources/images/intro_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Rectangle {
        id: intro_rect
        width: parent.width*0.5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10

        border {
            id: intro_rect_border
            width: 2
            color: "khaki"
        }

        color: "coral"
        opacity: 0.7

        radius: 20

        ColumnLayout {

            anchors.fill: parent
            anchors.margins: 20
            spacing: 40
            anchors.topMargin: 60
            anchors.bottomMargin: 60

            Image {
                source: Qt.resolvedUrl("resources/images/SyroySync.png")
                fillMode: Image.PreserveAspectFit

                Layout.fillWidth: true
                Layout.preferredHeight: width * 0.3
                Layout.alignment: Qt.AlignCenter
            }

            Text {
                text: "Syroy Sync - Lion"
                font.family: stdF.name
                font.pixelSize: 60
                color: "black"

                Layout.alignment: Qt.AlignCenter
                width: parent.width
                //Layout.bottomMargin: 0
            }

            Text {
                Layout.topMargin: 0
                text: "A SyroyCloud Project"

                font {
                    family: stdF.name
                    pixelSize: 32
                    italic: true
                    bold: true
                }

                color: "black"

                Layout.alignment: Qt.AlignCenter
                width: parent.width
            }

            Item { Layout.fillHeight: true }

            Button  {
                background: Rectangle {
                    color: "blue"
                    radius: 12
                }

                text: "Next"
                font.pixelSize: 48
                font.family: stdF.name

                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: parent.width * 0.4
                Layout.preferredHeight: font.pixelSize * 1.5

                palette.buttonText: "white"

                padding: 40

            }

            Text {
                text: "Version 2.0"
                font.family: stdF.name
                font.pixelSize: 32
                color: "black"

                Layout.alignment: Qt.AlignCenter
                width: parent.width
            }

            Text {
                text: "©2026 SyroySync. All rights reserved."
                font.family: stdF.name
                font.pixelSize: 18
                color: "black"

                Layout.alignment: Qt.AlignCenter
                width: parent.width
            }

        }
    }
}

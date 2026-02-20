import QtQuick
import QtQuick.Controls

Item {
    id: intro_panel

    Image {
        source: Qt.resolvedUrl("resources/images/intro_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Rectangle {
        id: intro_rect
        width: parent.width*0.6
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

        Column {

            anchors.horizontalCenter: parent.horizontalCenter
            padding: 20
            spacing: 60

            Image {
                source: Qt.resolvedUrl("resources/images/SyroySync.png")
                fillMode: Image.PreserveAspectFit
                width: intro_rect.width*0.5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Syroy Sync - Lion"
                font.family: stdF.name
                font.pixelSize: 60
                color: "black"
            }

            Button  {
                background: Rectangle {
                    color: "blue"
                    width: intro_rect.width*0.5
                }

                text: "Next"
                font.pixelSize: 48

            }

            Text {
                text: "Version 2.0"
                font.family: stdF.name
                font.pixelSize: 30
                color: "black"
            }

            Text {
                text: "©2026 SyroySync. All rights reserved."
                font.family: stdF.name
                font.pixelSize: 15
                color: "black"
            }

        }
    }
}

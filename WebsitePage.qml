import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Item {

    id: website_panel

    Image {
        source: Qt.resolvedUrl("resources/images/male_lion.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Rectangle {
        width: parent.width*0.9
        height: parent.height*0.9
        anchors.centerIn: parent

        radius: 10

        color: "midnightblue"
        opacity: 0.7

        ColumnLayout {

            anchors.fill: parent
            anchors.margins: 20
            spacing: 40

            Text {
                text: "Please enter link: "
                font.family: stdF.name
                font.pixelSize: 48
                color: "lightcyan"

                Layout.alignment: Qt.AlignLeft
                width: parent.width
            }
            RowLayout

            {
               spacing: 40

                TextField {
                    placeholderText: "Enter link here"
                    Layout.fillWidth: true

                    height: 60
                    font.pixelSize: 36

                    background: Rectangle {
                        color: "lightgrey"
                        radius: 15
                        border.color: "yellow"
                        border.width: 2
                    }
                }

                Image {
                    source: Qt.resolvedUrl("resources/images/next_arrow.png")
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 60
                }

            }


        }
    }

}

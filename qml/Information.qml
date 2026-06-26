import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: infoPage

    Image {
        source: Qt.resolvedUrl("resources/images/info_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 20

        color: "navy"
        opacity: 0.5

        radius: 20

        ColumnLayout {
            anchors.fill: parent

            Image {
                source: Qt.resolvedUrl("resources/images/SyroySync.png")
                fillMode: Image.PreserveAspectFit

                Layout.fillWidth: true
                Layout.preferredHeight: infoPage.height * 0.25
                Layout.alignment: Qt.AlignHCenter

                opacity: 1.0
            }

            Text {
                text: "SyroySync - Lion Edition"
                font.family: stdF.name
                font.pixelSize: 60
                color: "black"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Terms of Use Agreement"

                font.family: stdF.name
                font.pixelSize: 32
                font.underline: true

                color: "dodgerblue"

                Layout.alignment: Qt.AlignHCenter


                MouseArea {
                    id: hoverArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: Qt.openUrlExternally("https://syroy.com/terms")
                }
            }
        }
    }
}

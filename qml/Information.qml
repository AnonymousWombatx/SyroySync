import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: infoPage

    Rectangle {
        anchors.fill: parent
        color: "navy"
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 20

        color: "deepskyblue"
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

            Button {
                text: "Terms of Use Agreement"

                background: Rectangle {
                    color: "transparent"
                }

                font.family: stdF.name
                font.pixelSize: 48

                Layout.alignment: Qt.AlignHCenter

                onClicked: Qt.openUrlExternally("https://sync.syroy.com/terms")
            }
        }
    }
}

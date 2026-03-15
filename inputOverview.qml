import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: input_overview_panel

    signal optionSelected(string name)

    Image {
        source: Qt.resolvedUrl("resources/images/input_overview_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 40
        anchors.topMargin: 60
        anchors.bottomMargin: 60

        Text {
            text: "Please choose an option to search for an item:"
            font.family: stdF.name
            font.pixelSize: 48
            color: "black"

            Layout.alignment: Qt.AlignCenter
            width: parent.width
        }

        RowLayout {

            spacing: 40

            Repeater {
                model: [
                    { recColor: "coral", name: "API" },
                    { recColor: "khaki", name: "Browser" },
                    { recColor: "aqua", name: "Website" }
                ]
                delegate: optionBox
            }

        }
    }




    Component {

        id: optionBox



        Rectangle{

            required property string recColor
            required property string name

            color: recColor

            opacity: 0.75

            radius: 10

            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout
            {

                 anchors.fill: parent

                Image {
                    source: Qt.resolvedUrl(`resources/images/${name}.png`)
                    fillMode: Image.PreserveAspectFit

                    Layout.fillWidth: true
                    Layout.preferredHeight: width * 0.3
                    Layout.alignment: Qt.AlignCenter
                }

                Text {
                    text: name
                    font.family: stdF.name
                    font.pixelSize: 32
                    color: "black"

                    Layout.alignment: Qt.AlignCenter
                    width: parent.width
                }

            }

            MouseArea {
                anchors.fill: parent
                onClicked:
                    input_overview_panel.StackView.view.push(`${name}Page.qml`)
            }


        }


    }
}

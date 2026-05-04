import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    property bool inputChanged: false
    property bool loadingFinished: true
    property int selectedItem: -1

    Image {
        source: Qt.resolvedUrl("resources/images/API_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Connections {
        target: service

        function onYoutubeUrlFinished() {
            loadingFinished = true
        }
    }

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 20
        anchors.topMargin: 60
        anchors.bottomMargin: 60

        spacing: 40

        Text {
            text: "Please enter a search term:"
            font.family: stdF.name
            font.pixelSize: 48
            color: "chartreuse"

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
        }

        RowLayout{

            spacing: 40
            Layout.fillWidth: true

            TextField {
                id: searchTerm

                Layout.fillWidth: true

                placeholderText: "Enter your search term here"

                font.family: stdF.name
                font.pixelSize: 32

                color: "orange"

                background: Rectangle {

                    radius: 5

                    border.color: "orangered"

                    border.width: 2

                    color: inputChanged ? "red" : "lightgreen"
                    opacity: 0.5
                }

                onTextChanged: inputChanged = true

            }

            RoundButton {
                text: "\u21D2"

                radius: 10

                visible: inputChanged


                onClicked: {
                    inputChanged = false
                    loadingFinished = false
                    service.searchSnippet (searchTerm.text, service.Videos)
                }
            }

            BusyIndicator {
                running: true
                Layout.preferredWidth: 75
                Layout.preferredHeight: 75
                Layout.alignment: Qt.AlignHCenter
                visible: !loadingFinished
            }

        }        

        ListView  {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true

            spacing: 8

            model: videoModel

            delegate: Item {

                width: ListView.view.width
                height: 120

                Rectangle {
                    id: outerRec
                    anchors.fill: parent
                    anchors.margins: 6
                    color: "mediumspringgreen"
                    radius: 8

                    border.color: selectedItem==index ? "darkorange" : "mediumspringgreen"
                    border.width: 3

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 6
                        spacing: 10

                        Image {
                            source: thumbnail

                            Layout.preferredHeight: 100
                            Layout.preferredWidth: 178

                            fillMode: Image.PreserveAspectFit
                        }

                        ColumnLayout {
                            Text {
                                text: title
                                color: "mediumblue"
                                font {
                                    family: stdF.name
                                    pixelSize: 24
                                    bold: true
                                }

                                elide: Text.ElideRight
                                Layout.fillWidth: true

                            }

                            Text{
                                text: channel
                                color: "mediumblue"
                                font {
                                    family: stdF.name
                                    pixelSize: 16
                                    bold: false
                                }

                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }


                            Text{
                                text: duration + " · " + views
                                color: "red"
                                font {
                                    family: stdF.name
                                    pixelSize: 16
                                    bold: false
                                }

                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Clicked item number:", index)
                        selectedItem = index
                    }
                    onDoubleClicked: {

                    }
                }

            }
        }

    }

}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Item {

    id: website_panel

    property bool inputChanged: false
    property bool loadingFinished: true
    property bool itemSelected: false
    property string selectedUrl: ""

    Connections {
        target: plugin

        function onDataReady() {
            loadingFinished = true
        }
    }

    function selectionHandler() {
        console.log ("Selected song with ID ", selectedUrl);
        //push next page and directly pass variable
        website_panel.StackView.view.push("SettingsPage.qml", {"selectedUrl": selectedUrl})
    }

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

                    id: searchLink

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

                    onTextChanged: inputChanged = true
                }

                Image {
                    source: Qt.resolvedUrl("resources/images/next_arrow.png")
                    fillMode: Image.PreserveAspectFit
                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 60

                    visible: inputChanged && loadingFinished

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            inputChanged = false
                            loadingFinished = false
                            plugin.getDataFromLink (searchLink.text)
                        }
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

                        border.color: itemSelected ? "darkorange" : "mediumspringgreen"
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

                            if (itemSelected) {
                                selectedUrl = url
                                selectionHandler()
                                return;
                            }

                            console.log("Clicked item number:", index)
                            itemSelected = true
                            selectedUrl = url

                        }
                        onDoubleClicked: {
                            selectedUrl = url
                            selectionHandler()
                        }

                    }

                }
            }


        }


    }

}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {

    id: downloadDrawer

    contentItem: ColumnLayout
    {
        anchors.fill: parent

        RoundButton {
            text: "<"
            font {
                family: stdF.name
                bold: true
                pixelSize: 24
            }

            Layout.alignment: Qt.AlignRight

            onPressed: downloadDrawer.close()

            Layout.margins: 10
        }

        Text {
            text: "All Downloads"

            font {
                family: stdF.name
                pixelSize: 32
                bold: true
                underline: true
            }

            color: "white"

            Layout.alignment: Qt.AlignLeft
            width: parent.width

            Layout.margins: 20
        }

        Text {
            text: "No Downloads available"

            font {
                family: stdF.name
                pixelSize: 24
                bold: true
                underline: true
            }

            color: "white"

            Layout.alignment: Qt.AlignLeft
            width: parent.width

            Layout.margins: 10

            visible: manager.count === 0
        }

        ListView {
            model: manager

            Layout.fillWidth: true
            Layout.fillHeight: true

            visible: manager.count > 0

            clip: true

            Layout.margins: 20

            delegate: Rectangle {
                id: delegate

                width: ListView.view.width
                height: ListView.view.height*0.2

                color: "black"
                opacity: 0.5

                border {
                    width: 2
                    color: "darkred"
                }

                radius: 10

                ColumnLayout {

                    anchors.fill: parent
                    anchors.margins: 10


                    RowLayout {

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Image {
                            source: thumbnail
                            fillMode: Image.PreserveAspectFit
                            Layout.preferredWidth: delegate.width * 0.2
                            Layout.fillHeight: true
                        }
                        Text {
                            text: name
                            color: "white"

                            elide: Text.ElideRight
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignLeft

                            font {
                                family: stdF.name
                                pixelSize: 24
                                bold: true
                                underline: true
                            }
                        }
                    }

                    RowLayout {

                        Layout.fillWidth: true

                        ProgressBar {
                            Layout.preferredWidth: delegate.width * 0.6
                            implicitHeight: 8
                            value: progress / 100.0

                            indeterminate: dwlState === "Paused"

                            background: Rectangle {
                                radius: 10
                                color: dwlState == "Error" || dwlState === "Stopped" ? "red" : dwlState == "Finished" ? "green" : dwlState == "Paused" ? "yellow" : "black"

                            }
                            contentItem: Item {
                                    Rectangle {
                                        color: "white"
                                    }
                            }

                        }

                        Text {
                            text: dwlState
                            color: "white"
                            Layout.fillWidth: true
                            font {
                                family: stdF.name
                                pixelSize: 16
                                bold: true
                            }
                        }
                    }

                    RowLayout {
                        RoundButton {
                            text: "X"
                            onPressed: {
                                manager.stopDownload(index)
                            }
                            enabled: dwlState !== "Error" || dwlState !== "Stopped"
                        }
                        RoundButton {
                            text: "||"
                            visible: dwlState  !== "Paused"
                            onPressed: {
                                manager.pauseDownload(index)
                            }
                            enabled: dwlState !== "Error" || dwlState !== "Stopped"

                        }
                        RoundButton {
                            text: "▶"
                            visible: dwlState  === "Paused"
                            onPressed: {
                                manager.resumeDownload(index)
                            }
                            enabled: dwlState !== "Error" || dwlState !== "Stopped"
                        }
                    }
                }
            }
        }

        Button {
            background: Rectangle {
                color: "white"
                radius: 12
            }

            palette.buttonText: "red"

            text: "Clear"
            font.pixelSize: 18
            font.family: stdF.name

            onClicked: manager.clearFinished()

            Layout.alignment: Qt.AlignHCenter
        }



    }


}

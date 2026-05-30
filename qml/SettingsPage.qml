import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {

    Component.onCompleted: console.log("video:", JSON.stringify(video), "thumbnail:", video ? video.thumbnail : "none")


    property string selectedId: ""
    property var video: videoModel.getById(selectedId)

    Image {
        source: Qt.resolvedUrl("resources/images/settings_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    RowLayout {

        anchors.fill: parent

        spacing: 20

        Rectangle {

            color: "orangered"
            opacity: 0.5

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10
            Layout.preferredWidth: (3*parent.width) / 10

            ColumnLayout {

                anchors.centerIn: parent

                ButtonGroup {
                    buttons: row.children
                }

                RowLayout {
                    id: row

                    Button {
                        text: "Audio"
                        background: Rectangle {
                            color: "mediumblue"

                            border{
                                color: "black"
                                width: 2
                            }
                        }


                    }
                    Button {
                        text: "Video"
                        background: Rectangle {
                            color: "darkgreen"

                            border{
                                color: "black"
                                width: 2
                            }
                        }
                    }
                    Button {
                        text: "Both"
                        background: Rectangle {
                            color: "azure"

                            border{
                                color: "black"
                                width: 2
                            }
                        }
                    }

                }

            }

        }

        Rectangle {
            color: "orangered"
            opacity: 0.7

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: (4*parent.width) / 10

            Image {
                source: video.thumbnail

                width: 100
                height: 178

                fillMode: Image.PreserveAspectFit
            }
        }

        Rectangle {
            color: "orangered"
            opacity: 0.5

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20
            Layout.preferredWidth: (3*parent.width) / 10
        }

    }

}

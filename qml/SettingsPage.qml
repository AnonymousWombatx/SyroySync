import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {

    property var formats: []
    property bool audioOnly: false


    property string selectedId: ""
    property var video: videoModel.getById(selectedId)

    Component.onCompleted: {
        console.log("video:", JSON.stringify(video), "thumbnail:", video ? video.thumbnail : "none")

        var xhr = new XMLHttpRequest()
        xhr.open ("GET", "qrc:/resources/data/extensions.json")
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                formats=JSON.parse(xhr.responseText)
            }
        }
        xhr.send()
    }

    // Filter based on selected mode
    function filteredFormats() {
        return formats.filter(function(f) {
            if (audioOnly) return f.audioOnly === true
            return f.audioOnly === false
        })
    }

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
                        text: "Video"

                        onClicked: audioOnly = false

                        background: Rectangle {
                            color: "mediumblue"                        

                            border{
                                color: "black"
                                width: 2
                            }
                        }


                    }
                    Button {
                        text: "Audio Only"

                        onClicked: audioOnly = true

                        background: Rectangle {
                            color: "darkgreen"                        

                            border{
                                color: "black"
                                width: 2
                            }
                        }
                    }

                }

                ComboBox {
                    Layout.fillWidth: true
                    model: filteredFormats()
                    textRole: "label"
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

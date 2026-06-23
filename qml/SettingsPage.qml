import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import QtQuick.Effects

import QtCore

import QtQuick.Dialogs

import Qt.labs.qmlmodels



Item {

    id: root

    property var formats: []
    property bool audioOnly: false


    property string selectedUrl: ""
    property var video: videoModel.getByUrl(selectedUrl)

    property var filteredFormats: availableFormats.filter(function(f) {
        return audioOnly ? f.audioOnly === true : f.audioOnly === false
    })
    property var selectedFormat: filteredFormats[formatDropDown.currentIndex]
    property string selectedExtension: selectedFormat.extension


    //Standard Path for the downloads
    property string downloadDir: video.playlist
        ? StandardPaths.writableLocation(StandardPaths.DownloadLocation) + "/SyroySync"
        : StandardPaths.writableLocation(StandardPaths.DownloadLocation) + "/SyroySync/" + video.title.replace(/[\\/:*?"<>|]/g, "_")


    property bool nextAvailable: false

    Image {
        source: Qt.resolvedUrl("resources/images/settings_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    RowLayout {

        anchors.fill: parent
        spacing: 10

        Rectangle {

            color: "orangered"
            opacity: 0.5

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 10
            Layout.preferredWidth: (3*parent.width)/10

            ColumnLayout {

                id: formatC

                anchors.centerIn: parent

                ButtonGroup {
                    buttons: row.children
                }

                RowLayout {
                    id: row

                    Layout.alignment: Qt.AlignHCenter

                    Button {
                        text: "Video"

                        implicitWidth: root.width*0.08
                        implicitHeight: root.height*0.05

                        onClicked: audioOnly = false

                        background: Rectangle {
                            color: "mediumblue"

                            border{
                                color: audioOnly? "black" : "yellow"
                                width: 2
                            }
                        }


                    }
                    Button {
                        text: "Audio Only"

                        implicitWidth: root.width*0.08
                        implicitHeight: root.height * 0.05

                        onClicked: audioOnly = true


                        background: Rectangle {
                            color: "darkgreen"

                            border{
                                color: audioOnly? "yellow" : "black"
                                width: 2
                            }
                        }
                    }

                }

                ComboBox {
                    id: formatDropDown
                    //Layout.fillWidth: true
                    model: filteredFormats
                    textRole: "label"

                    implicitWidth: root.width*0.20
                    implicitHeight: root.height * 0.05

                    //change layout and formatting
                    contentItem: Text {
                        text: parent.displayText
                        color: "white"
                        font.family: stdF.name
                        font.pixelSize: 14
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 10
                    }

                    background: Rectangle {
                        color: audioOnly ? "darkgreen" : "mediumblue"
                        radius: 4
                        border.color: "black"
                        border.width: 2
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

            clip: true

            ColumnLayout {

                anchors.fill: parent

                spacing: 30


                TextArea {

                    id: videoName

                    text: "%(title)s - %(uploader)s"

                    Layout.fillWidth: true
                    Layout.preferredHeight: root.height * 0.15

                    Layout.topMargin: 20

                    wrapMode: TextArea.Wrap

                    font.pixelSize: 24

                    color: "white"

                    background: Rectangle {
                        color: "darkslategray"
                        radius: 10
                        border.color: "red"
                        border.width: 2
                        opacity: 0.8
                    }

                }



                Image {
                    id: thumbImage
                    source: video.thumbnail
                    fillMode: Image.PreserveAspectFit
                    visible: true

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Layout.leftMargin: 10
                    Layout.rightMargin: 10

                    Layout.alignment: Qt.AlignCenter
                }

                RowLayout {

                    Layout.alignment: Qt.AlignHCenter

                    CheckBox {
                        id: addThumbnail
                        text: "Add Thumbnail"
                        font.family: stdF.name
                    }
                    CheckBox {
                        id: replaceThumbnail
                        text: "Replace Video with Thumbnail"
                        font.family: stdF.name
                    }

                }

                RowLayout {

                    Layout.fillWidth: true
                    Layout.preferredHeight: root.height*0.1

                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                    Layout.bottomMargin: 20


                    TextArea {
                        id: inputPath

                        text: downloadDir

                        Layout.fillWidth: true
                        Layout.minimumHeight: root.height*0.1

                        wrapMode: TextArea.Wrap

                        font.pixelSize: 18

                        color: "white"

                        background: Rectangle {
                            color: "darkslategray"
                            radius: 10
                            border.color: "red"
                            border.width: 2
                            opacity: 0.8
                        }
                    }

                    Image {
                        source: Qt.resolvedUrl("resources/images/folder.png")
                        fillMode: Image.PreserveAspectFit
                        Layout.preferredHeight: root.height/10
                        Layout.preferredWidth: root.height/10

                        MouseArea {
                            anchors.fill: parent

                            onClicked: folderDialog.open()
                        }
                    }
                }

                Button {
                    text: "Download"

                    Layout.alignment: Qt.AlignHCenter

                    Layout.bottomMargin: 20

                    font {
                        family: stdF.name
                        pixelSize: 32
                    }

                    background: Rectangle {
                        color: "mediumblue"
                        radius: 15
                        border{
                            color: "yellow"
                            width: 2
                        }
                    }

                    onPressed: {
                        manager.addDownload ({
                            name: video.title + "-"+video.channel,
                            audioOnly: audioOnly,
                            extension: selectedExtension,
                            outputName: videoName.text,
                            saveLocation: downloadDir,
                            thumbnail: video.thumbnail,
                            addThumbnail: addThumbnail.checked,
                            replaceVideoWithThumbnail: replaceThumbnail.checked,
                            enableTrim: trimEnabled.checked,
                            trimStart: startTime.text,
                            trimEnd: endTime.text,
                            url: video.url
                            }, {
                            title: metaDialog.mTitle,
                            artist: metaDialog.mArtist,
                            album: metaDialog.mAlbum,
                            genre: metaDialog.mGenre,
                            releaseDate: metaDialog.mReleaseDate,
                            })
                        nextAvailable = true
                    }
                }

                Button {
                    text: "Next"
                    enabled: nextAvailable

                    Layout.alignment: Qt.AlignHCenter

                    Layout.bottomMargin: 20

                    font {
                        family: stdF.name
                        pixelSize: 32
                    }

                    background: Rectangle {
                        color: "mediumblue"
                        radius: 15
                        border{
                            color: "yellow"
                            width: 2
                        }
                    }

                    onPressed: intro_panel.StackView.view.push("inputOverview.qml")
                }

            }
        }

        Rectangle {
            color: "orangered"
            opacity: 0.5

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20
            Layout.preferredWidth: (3*parent.width) / 10

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 30

                CheckBox {
                    id: trimEnabled
                    text: "Trim"
                    Layout.alignment: Qt.AlignHCenter

                    contentItem: Text {
                        color: "black"
                        text: trimEnabled.text
                        leftPadding: trimEnabled.indicator.width + trimEnabled.spacing
                        verticalAlignment: Text.AlignVCenter
                    }

                    //indicator: Rectangle { color: "black" }

                    onPressed: endTime.text = video.duration
                }

                RowLayout {

                    TextField {
                        id: startTime
                        placeholderText: "00:00:00"
                        enabled: trimEnabled.checked
                        validator: RegularExpressionValidator {
                            regularExpression: /^\d+:[0-5]\d:[0-5]\d$|^\d+:[0-5]\d$/
                        }

                        color: trimEnabled.checked ? "black" : "white"

                        font {
                            family: stdF.name
                            pixelSize: 16
                        }

                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "→"
                        color: trimEnabled.checked ? "black" : "white"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    TextField {
                        id: endTime
                        text: video.duration
                        enabled: trimEnabled.checked
                        validator: RegularExpressionValidator {
                            regularExpression: /^\d+:[0-5]\d:[0-5]\d$|^\d+:[0-5]\d$/
                        }

                        color: trimEnabled.checked ? "black" : "white"

                        font {
                            family: stdF.name
                            pixelSize: 16
                        }

                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                Button {

                    text: "Metadata"

                    font {
                        family: stdF.name
                        pixelSize: 16
                        bold: true
                    }

                    background: Rectangle {
                        color: "cornflowerblue"

                        radius: 5

                        border{
                            width: 2
                        }
                    }

                    onPressed: metaDialog.open()

                    Layout.alignment: Qt.AlignHCenter

                }

            }

        }

    }

    FolderDialog {
        id: folderDialog
        onAccepted: inputPath.text = selectedFolder
        currentFolder: inputPath.text
    }

    Dialog {
        id: metaDialog

        property string mTitle: video.title
        property string mArtist: video.channel
        property string mAlbum: ""
        property string mGenre: ""
        property string mReleaseDate: ""

        title: "Metadata"

        standardButtons: Dialog.Save | Dialog.RestoreDefaults |Dialog.Cancel

        onReset: {
            tTitle.text = video.title
            tArtist.text = video.channel
            tAlbum.text = ""
            tGenre.text = ""
            tReleaseDate.text = ""
        }

        onAccepted: {
            mTitle = tTitle.text
            mArtist = tArtist.text
            mAlbum = tAlbum.text
            mGenre = tGenre.text
            mReleaseDate = tReleaseDate.text
        }

        onClosed: {
            tTitle.text = mTitle
            tArtist.text = mArtist
            tAlbum.text = mAlbum
            tGenre.text = mGenre
            tReleaseDate.text = mReleaseDate
        }

        GridLayout {

            columns: 2

            Text {text: "Title"; color: "white"}
            TextField {id: tTitle; text: metaDialog.mTitle}

            Text {text: "Artist"; color: "white"}
            TextField {id: tArtist; text: metaDialog.mArtist}

            Text {text: "Album"; color: "white"}
            TextField {id: tAlbum; text: metaDialog.mAlbum}

            Text {text: "Genre"; color: "white"}
            TextField {id: tGenre; text: metaDialog.mGenre}

            Text {text: "Release Date"; color: "white"}
            TextField {id: tReleaseDate; text: metaDialog.mReleaseDate}
        }
    }

}

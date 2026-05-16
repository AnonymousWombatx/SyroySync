import QtQuick
import QtQuick.Layouts

Item {

    Component.onCompleted: console.log("video:", JSON.stringify(video), "thumbnail:", video ? video.thumbnail : "none")


    property string selectedId: ""
    property var video: videoModel.getById(selectedId)

    Image {
        source: Qt.resolvedUrl("resources/images/settings_background.jpg")
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Image {
        source: video.thumbnail

        width: 100
        height: 178

        fillMode: Image.PreserveAspectFit
    }

}

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: applet
    width: 20
    height: 40
    x: 0
    y: 0
    color: "transparent"
    property string image: ""

    Image {
        id: appIcon
        source: "file:" + image
        anchors.fill: parent
        antialiasing: true
        fillMode: Image.PreserveAspectFit
    }
}

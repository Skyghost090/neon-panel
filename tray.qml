import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: trayIcon
    width: 20
    height: 40
    x: 0
    y: 0
    color: "transparent"
    property int winId: 0

    Image {
        id: appIcon
        source: "image://pixmap/" + winId
        anchors.fill: parent
        antialiasing: true
        fillMode: Image.PreserveAspectFit
    }
}

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: applicationInfo
    width: 40
    height: 40
    color: "transparent"

    property string nome: ""
    property string pidname: ""
    property bool minimize: true
    property var destak: destak
    property bool tooTip: true
    property int winId: 0

    signal create
    signal destroy

    onCreate: {
        destak.visible = true
    }

    onDestroy: {
        destak.visible = false
    }

    Rectangle {
        id: destak
        x: 0
        y: parent.height - 1.5
        height: 1.5
        width: parent.width
        color: main.detailColor
        visible: false
    }

    Rectangle {
        id: bgOpc
        anchors.fill: parent
        color: main.detailColor
        opacity: 0.0
    }

    Image {
        id: appIcon
        source: "image://pixmap/" + winId
        anchors.fill: parent
        anchors.margins: 6
        antialiasing: true
    }

    MouseArea {

        anchors.fill: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            if (mouse.button & Qt.LeftButton) {

                showAppInfo.visible = false

                openMenu = false
                main.clickOpc = main.startOpc

                if (!Context.isMinimized(pidname) & Context.isActive(pidname)) {
                    Context.manyMinimizes(pidname)
                    minimize = false;
                } else {
                    Context.manyActives(pidname)
                    minimize = true;
                }

            } else {

                showAppInfo.winIds = Context.windowsBywmclass(pidname)
                showAppInfo.y = main.y - 40
                showAppInfo.setText()
                showAppInfo.visible = true

                clickOpc = startOpc
                openMenu = true
            }
        }

        hoverEnabled: true

        onHoveredChanged: {
            bgOpc.opacity = 0.5
        }

        onExited: {
            bgOpc.opacity = 0.0
        }

    }
}

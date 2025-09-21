import QtQuick 2.3
import QtQuick.Window 2.15
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.15

import "utils.js" as Utils


ApplicationWindow {
    id: main
    visible: false
    x: 0
    y: 0
    width: 1000
    height: Context.parseConfigInt("barHeight")
    title: qsTr("Neon Painel")

    color: "transparent"

    // property alias rectangle10: rectangle10
    flags: Qt.Tool | Qt.FramelessWindowHint | Qt.WindowDoesNotAcceptFocus

    // property var neonMenu: Object
    property string color_: Context.parseConfigString("backgroundColor")
    property string detailColor: Context.parseConfigString("accent")
    property double opc: Context.parseConfigInt("backgroundOpacity") / 100
    property double clickOpc: 0.0
    property double startOpc: 0.0
    property int mainId: 0
    property bool openMenu: false

    property var showAppInfo: Object
    property var acessoRapido: Object

    property int launcherX: 0
    property var launcher: []

    property int subLauncherX: 0
    property int subLauncherX2: 0
    property var subLauncher: []
    property bool subLauncherStarted: true
    property bool windowVerify: false
    property bool firstApp: true
    property bool trayOpened: false

    signal desktopWindow(string _nome, string wmclass, int winId)
    signal clearWindows

    function clearWindow() {
        if (windowVerify) {

            for (var i = 0; i < subLauncher.length; i++) {
                subLauncher[i].destroy()
            }

            subLauncher = []
            subLauncherStarted = true
            separatorBar.visible = false
            windowVerify = false

            if (launcher.length > 0) {
                subLauncherX = 10
                subLauncherX2 = 0
            } else {
                subLauncherX = 0
                subLauncherX2 = 0
            }
        }
    }

    onClearWindows: {

        for (var i = 0; i < subLauncher.length; i++) {
            subLauncher[i].destroy()
        }

        subLauncher = []
        subLauncherStarted = true
        separatorBar.visible = false
        windowVerify = false

        if (launcher.length > 0) {
            subLauncherX = 0
            subLauncherX2 = 0
        } else {
            subLauncherX = 0
            subLauncherX2 = 0
        }
    }

    onDesktopWindow: {
        clearWindow()

        var fixicede = false
        subAppbar.width = 50
        subAppbar.height = main.height
        if (Context.parseConfigString("taskbarPosition") == "center"){
            var degressX = subAppbar.width / 2
            subAppbar.x = Screen.width / 2
        }

        if (Context.parseConfigString("taskbarPosition") == "start"){
            var degressX = subAppbar.width
            subAppbar.x = main.height
        }

        for (var j = 0; j < launcher.length; j++) {
            if (launcher[j].pidname === wmclass) {
                fixicede = true
            }
        }

        for (var t = 0; t < subLauncher.length; t++) {
            if (subLauncher[t].pidname === wmclass) {
                fixicede = true
            }
            subAppbar.width += 50
            if (Context.parseConfigString("taskbarPosition") == "center")
                subAppbar.x -= degressX
        }

        if (_nome !== "" && !fixicede) {

            if (subLauncherStarted) {

                if (launcher.length > 0) {
                    subLauncherX = 10
                    subLauncherX2 = 0
                } else {
                    subLauncherX = 0
                    subLauncherX2 = 0
                }

                subLauncherStarted = false
            }

            var compon = Qt.createComponent("launchers/Applications.qml")
            var obj = compon.createObject(subAppbar, {'x': subLauncherX, 'y': 0, 'nome': _nome, 'pidname': wmclass, 'winId': winId})

            obj.height = main.height
            obj.width = main.height
            if (subAppbar.x + subLauncherX + main.height + 5 < plugins.x) {
                btnShowMore.visible = false
                subLauncherX += main.height + 5

                if (Context.parseConfigString("taskbarPosition") == "center")
                    subAppbar.x -= degressX

                    if (!btnShowMore.moreArea) {
                        btnShowMore.moreArea = true
                        verticaline.height -= main.height + 5
                        btnShowMore.rotation = 0
                        main.y += main.height + 5
                        main.height -= main.height + 5
                        // accessSpeed.y = 0
                        Context.showMoreWindows(mainId, main.height + 5)
                    }

            } else {
                btnShowMore.visible = true
                subLauncherX2 += main.height + 5
                subAppbar.x -= degressX
            }


            subLauncher.push(obj)
            obj.destak.visible = true
            if (launcher.length > 0) separatorBar.visible = true
        }
    }

    MouseArea {
        // blank space panel click
        id: mouseMain
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        onClicked: {
            openMenu = false
            clickOpc = startOpc
            showAppInfo.visible = false
        }
    }

    Rectangle {
        id: mainRectangle
        anchors.fill: parent
        opacity: opc
        color: color_
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            id: btnMenuSub
            x: 2
            y: 4
            width: 34
            height: 32
            color: "#000000"
            opacity: clickOpc
            radius: 0
            border.width: 0
        }

        Rectangle {
            id: newMenuBtn
            width: main.height + 5
            height: main.height + 5
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                onClicked:
                    Context.exec("synapse")
                }
            }

            Image {
                source: "qrc:/icons/fiber_manual_record_80dp_E3E3E3_FILL0_wght400_GRAD200_opsz48.png"
                x: 5
                y: 5
                width: main.height - 10
                height: main.height - 10
            }
        }

        Item {
            id: applicationBar
            x: 42
            y: 0
            z: 9
            width: 0
            height: 40
        }

        Item {
           id: subAppbar
           x: applicationBar.x + applicationBar.width
           width: 3
           height: 40

           Rectangle {
              id: separatorBar
              x: 0
              y: 5
              width: 1
              height: 30
              color: "#ffffff"
              opacity: 0.0
              visible: false
           }
        }

        Item {
           id: subAppbar2
           y: 40
           x: 0
           width: plugins.x
           height: 40

           Rectangle {
               y: 1
               x: 0
               width: plugins.x
               height: 1
               color: "#ffffff"
               opacity: 0.0
           }

           Rectangle {
              id: separatorBar2
              x: 0
              y: 5
              width: 1
              height: 30
              color: "#ffffff"
              opacity: 0.0
              visible: false
           }
        }

        Rectangle {
            id: plugins
            x: parent.width - 95
            y: 0
            height: parent.height
            width: 110
            color: "transparent"

            Rectangle {
                id: verticaline
                x: 0
                y: 0
                height: 40
                width: 20
                color: "transparent"

                Image {
                    id: btnShowMore
                    y: (parent.height / 2) - (height / 2)
                    height: 20
                    width: 20
                    source: "file://" + Context.basepath + '/icon-down.png'
                    rotation: 0
                    visible: true

                    property bool moreArea: true

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {

                            // acessoRapido.visible = false

                            if (parent.moreArea) {

                                main.y -= 40
                                btnShowMore.moreArea = false
                                verticaline.height += 40
                                btnShowMore.rotation = 180
                                main.height += 40
                                // accessSpeed.y = 19
                                Context.showMoreWindows(mainId, 80)

                            } else {

                                btnShowMore.moreArea = true
                                verticaline.height -= 40
                                btnShowMore.rotation = 0
                                main.y += 40
                                main.height -= 40
                                // accessSpeed.y = 0
                                Context.showMoreWindows(mainId, 40)
                            }

                            y = (parent.height / 2) - (height / 2)
                        }
                    }
                }
            }

            Text {
                id: clock
                x: 102
                text: Utils.getTime()
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 11
                anchors.top: parent.top
                anchors.topMargin: 12
                anchors.right: parent.right
                anchors.rightMargin: 35
                font.pointSize: main.height / 4
                color: "#ffffff"

                Timer {
                    interval: 100
                    running: true
                    repeat: true
                    onTriggered: {
                        var d = Utils.getTime().split('|')
                        clock.text = d[0]
                        // acessoRapido.acessText.text = d[1]
                    }
               }
            }
    }
    Rectangle {
        id: showSystray
        x: main.width - 10
        height: main.height
        width: 10
        color: "#FFFFFF"
        opacity: 0.15
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(trayOpened == false){
                    Context.exec("stalonetray -t true")
                    trayOpened = true
                } else {
                    Context.exec("killall stalonetray")
                    trayOpened = false
                }
            }
        }
    }

    Component.onCompleted: {
        // event click listener
        var info = Qt.createComponent("qrc:/launchers/appShowInfo.qml")
        showAppInfo = info.createObject(applicationBar)
        showAppInfo.visible = false
        main.visible = true
    }
}

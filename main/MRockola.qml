/*
 * MRockola.
 * Copyright (C) 2011-2017  Edgar Armando Lopez Hernandez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: inge_lopez@yahoo.com
 */

import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import MRockola.DBase 1.0
import MRockola.MPlayer 1.0
import MRockola.Settings 1.0
import QtQuick.Window 2.0

import "../components"

ApplicationWindow {
    title: "MRockola"
    visible: true
    width: Screen.desktopAvailableWidth ; height: Screen.desktopAvailableHeight
    //width: 800; height: 640
    //width: 1920; height: 1080

 Item {
     id: mainWindow
     anchors.fill: parent
     property alias setFoundCover: gridView.currentIndex;

     Rectangle {
        anchors.fill: parent
        focus: true
        Background {
            id: mainBackgroundItem
        }
        Text {
            id: configText
            visible: false
            anchors.centerIn: parent
            text: qsTr("PRESS <b>F2</b> TO CONFIGURE")
            font.pointSize: 20
            color: "white"
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        ColumnLayout {
            spacing: 2
            anchors.fill: parent
            Rectangle {
                id: mainLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: (parent.height * 0.80)
                color: "transparent"
                GridView {
                    id: gridView
                    anchors.fill: mainLayout
                    cellWidth:  (width * 0.50)
                    cellHeight: (height * 0.50)
                    model: database
                    delegate: Cover {}
                    flow: GridView.TopToBottom
                    focus: true
                    snapMode: GridView.SnapToRow
                    keyNavigationWraps: true

                    add: Transition {
                        NumberAnimation { properties: "x,y"; from: 100; duration: 1000 }
                    }
                    enabled: !finder.isVisible  //finder show disable or enable this
                }
            }
            Rectangle {
                id: controlBarRec
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: parent.height * 0.16
                radius: 10
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "lightslategrey" }
                    GradientStop { position: 1.0; color: "black" }
                }
                border.color: "transparent"
                DisplayBar {
                    id: displayBarItem
                }
            }
            Rectangle {
                id: bar_menu
                color: "transparent"
                opacity: 0.5
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: parent.height * 0.04
                ControlBar {}
            }
        }
        Rectangle {
            id: player
            anchors.fill: parent
            anchors.margins: 5
            anchors.bottomMargin: bar_menu.height
            visible: false
            MediaPlayer {
                id: mediaPlayer
                anchors.fill: parent
                onIsVideo: mainWindow.playVideo(b);
                onFinishTrack: displayBarItem.nextPlay();
                onProgressBarSetMax: { displayBarItem.progressBarMax = max }
                onProgressBarSetValue: { displayBarItem.progressBarValue = value }
            }
            MouseArea {
                id: mousearea
                anchors.fill: parent
                onClicked: {
                    mediaPlayer.playNext()
                }
            }
        }
        Finder {
            id: finder
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -(parent.height * 0.08)
        }
        CoverDetail {
            id: coverDetail
        }
        Search {
            id: search
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -(parent.height * 0.08)
        }
        DataBase {
            id: database
            onStepMaxCount: mainSetup.progressBarMax(max);
            onStepLoading: mainSetup.progressBarUpdate(step);
            onStepLoadingDone: mainSetup.progressBarDone();
            onSetConfigText: mainWindow.setMessageConfig(b);
        }
        Settings {
            id: settings
        }

        MRSetup {
            id: mainSetup
            anchors.fill: parent
            visible: false
        }
    }

    Keys.onPressed: {

        switch( settings.validateKey(event.key) ) {
            case Settings.KEY_SKIP:
                mediaPlayer.playNext()
                break;
            case Qt.Key_P:
                //event.accepted = true
                //mediaPlayer.setQueue("/home/isaac/backuplaptop/backup/karaoke/espanol2/22052.mp3", 2)
                break;
            case Settings.KEY_LEFT:
                if(!coverDetail.isVisible)
                    gridView.moveCurrentIndexLeft()
                break;
            case Settings.KEY_RIGHT:
                if(!coverDetail.isVisible)
                    gridView.moveCurrentIndexRight()
                break;

            case Settings.KEY_0:
            case Settings.KEY_1:
            case Settings.KEY_2:
            case Settings.KEY_3:
            case Settings.KEY_4:
            case Settings.KEY_5:
            case Settings.KEY_6:
            case Settings.KEY_7:
            case Settings.KEY_8:
            case Settings.KEY_9:
                if(!search.isVisible)
                    search.show()
                if(search.number.length < 4 )
                    search.number += event.text

                if(search.number.length === 4) {
                    if(!coverDetail.isVisible) {
                        if(gridView.count > search.number) {
                            gridView.currentIndex = search.number
                            gridView.currentItem.openCoverDetail();
                        }
                    }
                    search.hide()
                }
                break;

            case Settings.KEY_SEARCH:
                if(!search.isVisible && !coverDetail.isVisible) {
                    finder.forceActiveFocus()
                    finder.show();
                }
                break;

            case Settings.KEY_DELETE:
                var str = search.number
                search.number = str.substring(0, str.length -1)
                if( str.length === 1 )
                    search.hide()
                break;

            case Settings.KEY_CONFIG:
                mainSetup.visible = true;
                break;
        }
    }

    function playVideo(isVideo)  {
        if(isVideo)
        {
            coverDetail.hide()
            controlBarRec.visible = false
            gridView.visible = false
            player.visible = true
        }
        else
        {
            controlBarRec.visible = true
            gridView.visible = true
            player.visible = false
        }
    }

    function setMessageConfig(setShow) {
        configText.visible = setShow
    }

    function selectCover() {
        gridView.visible = false
    }

    function startupFunction() {
        database.loadCovers()
    }

    function updateFoundCover() {
        //if(gridView.currentIndex == 1)
            gridView.moveCurrentIndexRight()
        //else
        //    gridView.moveCurrentIndexLeft()
    }

    Component.onCompleted: startupFunction();
 }
}

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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import MRockola.DBase 1.0
import MRockola.MPlayer 1.0
import MRockola.Settings 1.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

import "../components"

ApplicationWindow {
    title: "MRockola"
    visible: true
    //width: Screen.desktopAvailableWidth ; height: Screen.desktopAvailableHeight
    width: 800; height: 600
    //width: 1920; height: 1080

 Item {
     id: mainWindow
     anchors.fill: parent
     property alias setFoundCover: gridView.currentIndex;
     property bool setPlayList: false
     property string getNameList: "none"

     // property int fullTime: 0


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
        objectName: "before main"
        anchors.fill: parent
        color: "transparent"
        ColumnLayout {
            spacing: 2
            anchors.fill: parent
            Rectangle {
                id: mainLayout
                objectName: "mainLayout"
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

                    // Show off... tirei por hora...
                    //add: Transition {
                        //NumberAnimation { properties: "x,y"; from: 100; duration: 1000 }
                    //}
                    //enabled: !finder.isVisible  //finder show disable or enable this
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
            objectName: "player"
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
        CenterDialog {
            id: centeredDialog
            // Layout.fillHeight: true
            // Layout.fillWidth: true
        }

        Finder {
            id: finder
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -(parent.height * 0.08)
        }

        Plist { id: playList
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -(parent.height * 0.08) }


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
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.fill: parent
            visible: false
        }
        LookAndFeelui {
            id: mainLook
            anchors.fill: parent
            visible: false
        }
    }
//    Keys.forwardTo: [playList]
//    focus: true

    Keys.onPressed: {

        switch( settings.validateKey(event.key) ) {

            case Settings.KEY_SKIP:
                mediaPlayer.playNext()
                /*
                centeredDialog.title = qsTr("ATTENTION!")
                centeredDialog.text = qsTr("COVERDETAIL -> "+mediaPlayer.getCurrentTime())
                centeredDialog.visible = true
                */
                break;
            case Settings.KEY_R: //do scan of networks in range
                 centeredDialog.title = qsTr("ATTENTION!")
                 centeredDialog.text = qsTr("RKEY ENABLED -> ")
                 console.debug("Test")
                 // var anArray = database.scan()
                 var anArray = database.get2PlayTable(51)
                 for (var i=0; i<anArray.length; i++)
                     console.log("Array item:", anArray[i])
                 break;
            case Settings.KEY_P: // Qt.Key_P:
                //event.accepted = true
                //mediaPlayer.setQueue("/home/isaac/backuplaptop/backup/karaoke/espanol2/22052.mp3", 2)
                console.log("geNameList", mainWindow.getNameList)
                if (mainWindow.setPlayList) {
                    centeredDialog.title = qsTr("ATTENTION!")
                    centeredDialog.text = qsTr("PLAYLIST DISABLED -> "+mainWindow.getNameList)
                    centeredDialog.visible = true
                    mainWindow.setPlayList = false
                    mainWindow.getNameList = "none"
                    // database.loadCovers();
                }
                else {
                    mainWindow.setPlayList = true
                    centeredDialog.title = qsTr("ATTENTION!")
                    // centeredDialog.text = qsTr("PlayID: "+database.storeList(0, "NONE", 0))
                    centeredDialog.text = qsTr("playlist mode enabled -> "+mainWindow.getNameList)
                    centeredDialog.visible = true
                }
                //if(!playList.isVisible && !coverDetail.isVisible) {
                    // playList.show()
                    // playList.forceActiveFocus() // tem de usar essa merda sempre depois de chamar o qml...
                 //}
                break;
            case Settings.KEY_LEFT:
                //
                if(gridView.currentIndex >= 2)
                  gridView.currentIndex = gridView.currentIndex - 2
                /*
                centeredDialog.title = qsTr("Error!")
                centeredDialog.text = qsTr("currentIndex: "+gridView.currentIndex)
                centeredDialog.visible = true
                */
                if(!coverDetail.isVisible)
                    gridView.moveCurrentIndexLeft()
                break;
            case Settings.KEY_RIGHT:
                // settings.popIt(gridView.currentIndex)
//                gridView.moveCurrentIndexDown()
                gridView.currentIndex = gridView.currentIndex + 2
                /*
                centeredDialog.title = qsTr("Error!")
                centeredDialog.text = qsTr("currentIndex: "+gridView.currentIndex)
                centeredDialog.visible = true
                */
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
                            gridView.currentItem.getAlbumDetail()

                            if (coverDetail.typeCover === 4) {
                              console.log("indexCover ->", coverDetail.indexCover)
                              coverDetail.playMixTape(coverDetail.indexCover)
                              // coverDetail.setTrack(coverDetail.indexCover)

                            }
                            else {
                              gridView.currentItem.openCoverDetail();
                            }

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
                console.debug ("[settings] -> CONFIG")
                console.log (event.text, parent.objectName.toString())

                mainSetup.visible = true;
                break;
            default:
                console.debug ("veio pra default")
                console.log (event.text)
                // centeredDialog.title = qsTr("fica zoado no OSX!")
                // centeredDialog.text = qsTr("NAO TEM NOME NESSA PARADA")
                // centeredDialog.visible = true
                if( event.text === 'q' )
                    Qt.quit()
                break;


                // console.log ("1: count is zero")
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

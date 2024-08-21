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
import "../main"
import MRockola.Settings 1.0 as Keyboard
// essa parada aqui de importar eh importante para nao se preocupar o setfocus e controlar as teclas daqui...

Item {
    id: playList
    property int max_character: 35
    property variant abc: ["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
    property int index: 10  //A
    property string character: "A"
    property bool isVisible: true
    property string listTypeStr: "Music"
    property int listType: 0

    opacity: 0
    // visible: opacity
    // visible: true
    // scale: 0.3

    width: parent.width * 0.20;
    height: parent.height * 0.40;

    Rectangle {
        anchors.fill: parent
        color:activeFocus?"lightsteelblue":"grey"; width: 240; height: 25
        border.color: "black"
        border.width: 2

        Text {
            visible: true
            anchors.centerIn: parent
            text:mainWindow.setPlayList?qsTr("ACTIVE"):qsTr("DISABLED")
            font.pointSize: 20
            color: "white"
        }

        Column {
            id: column1
            anchors.fill: parent
            Rectangle {
                width: playList.width
                height: playList.height / 4
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "black"
                // border.width: 2
                color:activeFocus?"lightsteelblue":"grey"
                //color: "transparent"
                Text {
                    id: configList
                    visible: true
                    anchors.centerIn: parent
                    text: qsTr("PRESS P")
                    font.pointSize: 20
                    color: "white"
                }

            }
        }
    }
    // O item é controlado por aqui por conta de usar a rotina keyboard ou pelo menos deveria...
    Keys.onPressed: {

        switch( settings.validateKey(event.key) ) {
            case Keyboard.Settings.KEY_P:
                playList.hide()
                break;
            case Keyboard.Settings.KEY_LEFT:
                 arrowLeftRigthClicked(false)
                break;
            case Keyboard.Settings.KEY_RIGHT:
                arrowLeftRigthClicked(true)
                break;
            case Keyboard.Settings.KEY_UP:
                arrowUpClicked()
                break;
            case Keyboard.Settings.KEY_ENTER:
                selectedLetter()
                break;

        }
        event.accepted = true
    }

    function show() {
        showAnimation.restart();
        isVisible = true
    }

    function hide() {
        hideAnimation.restart();
        isVisible = false
        mainWindow.forceActiveFocus()
    }

    function arrowUpClicked() {
         listType++

        if(listType == 1)
            listTypeStr = qsTr("Video")
        else if(listType == 2)
            listTypeStr = qsTr("Karaoke")
        else {
            listType = 0;
            listTypeStr = qsTr("Music")
        }

    }

    function arrowLeftRigthClicked(isRight) {
		if(isRight)	{
            index = (++index > max_character? 0 : index )
		} else  {
            index = (--index >= 0 ? index : max_character)
        }
        character = abc[index]
    }

    function selectedLetter() {
        var position = database.getFoundAlbum(abc[index] , listType);
        if(position) {
            mainWindow.setFoundCover = position
            mainWindow.updateFoundCover()
        }
        playList.hide()
    }

    ParallelAnimation {
        id: showAnimation
        NumberAnimation { target: playList; property: "opacity"; to: 1.0; duration: 500; easing.type: Easing.InOutQuad }
        NumberAnimation { target: playList; property: "scale"; to: 1.0; duration: 500; easing.type: Easing.OutBounce }
    }
    ParallelAnimation {
        id: hideAnimation
        NumberAnimation { target: playList; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
        NumberAnimation { target: playList; property: "scale"; to: 0.3; duration: 500; easing.type: Easing.OutBack }
    }
}

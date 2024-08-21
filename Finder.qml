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
import MRockola.Settings 1.0 as Settings

Item {
    id:  finderView
    property int max_character: 35
    property variant abc: ["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
    property int index: 10  //A
    property string character: "A"
    property bool isVisible: false
    property string listTypeStr: "music"
    property int listType: 0

    opacity: 0
    visible: opacity
    scale: 0.3

    width: parent.width * 0.20;
    height: parent.width * 0.40;

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Column {
            id: column1
            anchors.fill: parent
            Rectangle {
                width: finderView.width
                height: finderView.height / 4
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "black"
                //color: "transparent"
                Image {

                    anchors.fill: parent
                    source: "qrc:/arrow-up"
                }
                Text {
                    color: "black"
                    text: listTypeStr
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    font.bold: true
                    font.pointSize: 16
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        arrowUpClicked(false)
                    }
                }
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                    width: finderView.width * 0.40
                    height: finderView.height / 2
                    border.color: "black"
                    Image {
                        anchors.fill: parent
                        source: "qrc:/arrow-left"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            arrowLeftRigthClicked(false)
                        }
                    }
                }
                Rectangle {
                    width: finderView.width * 0.50
                    height: finderView.height / 2
                    border.color: "blue"
                    color: "black"
                    Text {
                        color: "white"
                        text: character
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: 36
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            selectedLetter()
                        }
                    }
                }
                Rectangle {
                    width: finderView.width * 0.40
                    height: finderView.height /2
                    border.color: "black"
                    Image {
                        anchors.fill: parent
                        source: "qrc:/arrow-right"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            arrowLeftRigthClicked(true)
                        }
                    }
                }
            }
        }
    }
    Keys.onPressed: {

        switch( settings.validateKey(event.key) ) {
            case Settings.KEY_SEARCH:
                hide();
                break;
            case Settings.KEY_LEFT:
                 arrowLeftRigthClicked(false)
                break;
            case Settings.KEY_RIGHT:
                arrowLeftRigthClicked(true)
                break;
            case Settings.KEY_UP:
                arrowUpClicked()
                break;
            case Settings.KEY_ENTER:
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
        finderView.hide()
    }

    ParallelAnimation {
        id: showAnimation
        NumberAnimation { target: finderView; property: "opacity"; to: 1.0; duration: 500; easing.type: Easing.InOutQuad }
        NumberAnimation { target: finderView; property: "scale"; to: 1.0; duration: 500; easing.type: Easing.OutBounce }
    }
    ParallelAnimation {
        id: hideAnimation
        NumberAnimation { target: finderView; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
        NumberAnimation { target: finderView; property: "scale"; to: 0.3; duration: 500; easing.type: Easing.OutBack }
    }
}

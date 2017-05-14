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
 * GNU General Public License for more open details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: inge_lopez@yahoo.com
 */

import QtQuick 2.5
import QtQuick.Controls 2.0
import MRockola.Settings 1.0 as Keyboard

Item {
    id: coverDetailView

    property int indexCover
    property int typeCover
    property string order
    property string artist
    property string album
    property string imagen
    property variant playlist

    property int position_x
    property int position_y
    property int sizeWidth
    property int sizeHeight

    property bool isVisible: false

    opacity: 0
    visible: opacity

    Rectangle {
        id: coverFrame
        anchors.fill: parent
        anchors.margins: 5
        color: "black"
        radius: 20
    }

    Label {
        id: artistName
        x: 14; y: 14
        width: parent.width - (parent.width / 4) - x
        maximumLineCount: width
        elide: "ElideLeft"
        color: "#ffffff"
        text: artist
        activeFocusOnTab: false
        font.italic: true
        font.pointSize: 16
        font.family: "Times New Roman"
    }

    Label {
        id: idNumber
        y: 14
        anchors.left: artistName.right
        width:  parent.width / 4
        color: "#ffffff"
        text: order
        font.bold: true
        font.pointSize: 16
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: coverImage
        x:10
        anchors.top: artistName.bottom
        anchors.topMargin: 10
        width:  parent.width / 2
        height: ((parent.height / 2) - y) * 2
        source: imagen;
    }

    Rectangle {
        anchors {
            left: coverImage.right
            top: idNumber.bottom
            right:  coverDetailView.right
            bottom: coverDetailView.bottom
            leftMargin: 10
            topMargin: 10
            bottomMargin: 20
            rightMargin: 20
        }
        color: "white"

        ListView {
            id: songlist
            anchors {
                fill: parent
                margins: 2
            }
            clip: true
            focus: true
            model: playlist
            delegate: Component {
                id: listRows
                Rectangle {
                    id: itemList
                    width: parent.width; height: songName.height
                    color: index % 2 == 0 ? "#dfe28b" : "white"
                    Text {
                        id: songName
                        color: "black"
                        text:  ("00"+ index ).slice(-2) + ".-" + modelData
                        font.bold: true
                        font.pointSize: 18
                        wrapMode: Text.WordWrap
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                             setTrack(index)
                        }
                    }
                }
            }
        }
    }
    Label {
        id: albumName
        x: 14
        anchors.top: coverImage.bottom
        anchors.topMargin: 10
        width: parent.width
        color: "#ffffff"
        text: album
        wrapMode: Text.NoWrap
        verticalAlignment: Text.AlignVCenter
        font.italic: true
        font.bold: true
        font.family: "Times New Roman"
        font.pointSize: 14
    }

    MouseArea {
        id: mouseAreaAllCover
        anchors.fill: coverImage
        onClicked: {
            coverDetailView.hide()
        }
    }

    Keys.onPressed: {
        switch( settings.validateKey(event.key) ) {
            case Keyboard.Settings.KEY_ENTER:
                coverDetailView.hide()
                break;
            case Keyboard.Settings.KEY_SKIP:
                mediaPlayer.playNext()
                break;
            case Keyboard.Settings.KEY_0:
            case Keyboard.Settings.KEY_1:
            case Keyboard.Settings.KEY_2:
            case Keyboard.Settings.KEY_3:
            case Keyboard.Settings.KEY_4:
            case Keyboard.Settings.KEY_5:
            case Keyboard.Settings.KEY_6:
            case Keyboard.Settings.KEY_7:
            case Keyboard.Settings.KEY_8:
            case Keyboard.Settings.KEY_9:
                if(!search.isVisible)
                    search.show()
                if(search.number.length < 2 )
                    search.number += event.text

                if(search.number.length === 2) {
                    var index = parseInt(search.number);
                     setTrack(index)
                    search.number = ""
                    search.hide()
                }
                break;

            case Keyboard.Settings.KEY_DELETE:
                var str = search.number
                search.number = str.substring(0, str.length -1)
                if( str.length === 1 )
                    search.hide()
                break;
        }
        event.accepted = true
    }

    function show() {
        coverDetailView.anchors.centerIn = undefined
        coverDetailView.anchors.fill = undefined
        coverDetailView.x = position_x
        coverDetailView.y = position_y
        coverDetailView.width = sizeWidth
        coverDetailView.height = sizeHeight
        coverDetailView.scale = 1.0
        showAnimation.restart();
        isVisible = true
    }

    function hide() {
        hideAnimation.restart();
        isVisible = false
        mainWindow.forceActiveFocus()
    }

    function setTrack(index) {
        if(index >= playlist.length)
           return;

        var songStr = index + ".-" +  playlist[index]
        displayBarItem.playListAdd(order,artist, album, songStr, imagen )
        mediaPlayer.setQueue(database.getPathTrack(indexCover, index), typeCover)
    }

    SequentialAnimation {
        id: showAnimation
        ParallelAnimation {
            NumberAnimation { target: coverDetailView; property: "x"; to: {position_x > 0?(position_x -  width/2) : (position_x + width)/2;} duration: 200; easing.type: Easing.Linear }
            NumberAnimation { target: coverDetailView; property: "y"; to: {position_y > 0?(position_y -  height/2) : (position_y + height)/2;} duration: 200; easing.type: Easing.Linear }
            NumberAnimation { target: coverDetailView; property: "opacity"; to: 1.0; duration: 200; easing.type: Easing.Linear }
        }
        ParallelAnimation {
            NumberAnimation { target: coverDetailView; property: "x"; to: 0; duration: 200; easing.type: Easing.Linear }
            NumberAnimation { target: coverDetailView; property: "y"; to: 0; duration: 200; easing.type: Easing.Linear }
            NumberAnimation { target: coverDetailView; property: "width"; to:  parent.width; duration: 200; easing.type: Easing.Linear }
            NumberAnimation { target: coverDetailView; property: "height"; to:  (parent.height * 0.80); duration: 200; easing.type: Easing.Linear }
        }
        PropertyAction { target: coverDetailView; property: "anchors.fill"; value: parent }
        PropertyAction { target: coverDetailView; property: "anchors.bottomMargin"; value: (parent.height * 0.20) }

    }

    ParallelAnimation {
        id: hideAnimation
        NumberAnimation { target: coverDetailView; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
        NumberAnimation { target: coverDetailView; property: "scale"; to: 0.3; duration: 500; easing.type: Easing.OutBack }
    }
}

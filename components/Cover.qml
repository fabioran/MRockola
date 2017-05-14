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

Item {
    id: coverView
    width: gridView.cellWidth
    height: gridView.cellHeight

    Rectangle {
        id: coverFrame
        anchors.fill: parent
        anchors.margins: 5
        color: "black"
        radius: 20
        opacity:0.9
    }

    Label {
        id: artistName
        x: 22; y: 14
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
        font.pointSize: 18
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: coverImage
        x:10
        anchors.top: artistName.bottom
        anchors.topMargin: 10
        width:  parent.width * 0.5
        height: ((parent.height / 2) - y) * 2
        source: (imagen === "NONE"? "qrc:/default" : "file:///" + imagen);
        asynchronous: true
    }

    Rectangle {
        anchors {
            left: coverImage.right
            top: idNumber.bottom
            right:  coverView.right
            bottom: coverView.bottom
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
                    id: banner
                    width: parent.width;
                    height: songName.height
                    color: index % 2 == 0 ? "#dfe28b" : "white"
                    clip: true
                    Text {
                        id: songName
                        color: "black"
                        text:  ("00"+ index ).slice(-2) + ".-" + modelData
                        font.bold: true
                        font.pointSize: 14
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }

    Label {
        id: albumName
        x: 22
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
           openCoverDetail()
        }
    }

    function openCoverDetail() {
        coverDetail.position_x = gridView.mapFromItem(coverView, 0, 0).x
        coverDetail.position_y = gridView.mapFromItem(coverView, 0, 0).y
        coverDetail.sizeWidth = width
        coverDetail.sizeHeight = height

        coverDetail.indexCover = id
        coverDetail.typeCover = type
        coverDetail.artist = artist
        coverDetail.order = order
        coverDetail.imagen = coverImage.source
        coverDetail.playlist = playlist
        coverDetail.album = album
        coverDetail.forceActiveFocus()
        coverDetail.show()
    }
}

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

Item {
    id: displayBarView

    property string number
    property string artistName
    property string albumName
    property string songName
    property string songImage
    property string credits : "FREE"
    property real progressBarMax : 100.0
    property real progressBarValue : 0.0
    property alias artistPlay: artistPlay

    anchors.fill: parent

    RowLayout {
        anchors.fill: parent
        spacing: 10
        Rectangle {
            id: screenTrackList
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: parent.width * 0.48
            radius: 10
            border.color: "transparent"
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#ffffff" }
                GradientStop { position: 0.066; color: "#07416c" }
                GradientStop { position: 1.0; color: "#2a75b7" }
            }
            ListView {
                anchors.fill: screenTrackList
                anchors.margins: 10
                clip: true
                model: songModel
                delegate: songDelegate
            }
        }
        RowLayout {
            id: trackplay
            Layout.maximumWidth: parent.width * 0.48
            Layout.fillWidth: true
            Layout.fillHeight: true
            Rectangle {
                id: albumImage
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumWidth: parent.height
                border.color: "black"
                color: "transparent"
                radius: 4
                Image {
                    anchors.fill: parent
                    anchors.margins: 4
                    source: songImage? songImage : "qrc:/default"
                    mipmap: true
                }
            }
            ColumnLayout {
                id: screenPlayList
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.left: albumImage.right
                anchors.leftMargin: 10

                Rectangle {
                    id: datas
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: parent.height * 0.70
                    color: "transparent"
                    ColumnLayout {
                        id: datasLayout
                        anchors.fill: parent
                        spacing: 2
                        Text {
                            id: artistPlay
                            text: artistName //"mrockola artist"
                            Layout.fillHeight: true
                            verticalAlignment: Text.AlignVCenter
                            color: "white"
                            font.pixelSize: 16
                            fontSizeMode: Text.Fit
                        }
                        Text {
                            id: text1
                            text:number +"  " + songName //"mrockola album"
                            Layout.fillHeight: true
                            verticalAlignment: Text.AlignVCenter
                            color: "white"
                            font.pixelSize: 14
                        }
                    }
                }
                Rectangle {
                    id: barProgress
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: parent.height * 0.30
                    color:"transparent"

                    Progressbar {
                     id: progress
                     anchors.centerIn: parent
                     value: progressBarValue
                     maximum: progressBarMax
                     }
                }
            }
        }
    }
    ListModel {
        id:songModel
        ListElement {
            number: ""
            artistName:""
            albumName: ""
            songName: ""
            songImage: ""
        }
    }
    Component {
        id: songDelegate
        Row {
            spacing: 8
            Text { text: number; color: "white" ; font.pointSize: 14}
            Text { text: songName; color: "white"; font.pointSize: 14 }
        }
    }

    function playListAdd(number, name, album, song, image)
    {
        songModel.append({"number":number,"artistName": name, "albumName":album, "songName":song, "songImage":image});
    }

    function nextPlay()
    {
        if(songModel.get(0)) {
            number = songModel.get(0).number;
            artistName = songModel.get(0).artistName;
            albumName = songModel.get(0).albumName;
            songName = songModel.get(0).songName;
            songImage = songModel.get(0).songImage;
            songModel.remove(0);
        } else {
            number = "";
            artistName = "";
            albumName = "";
            songName = "";
            songImage = "";
        }
    }
    Component.onCompleted: {songModel.clear();} //fix clean list
}

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

import QtQuick 2.3
import QtQuick.Controls 1.2
import "../main"
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
//    property variant getlist
    property variant getMlist
//    property variant myList1

    property int position_x
    property int position_y
    property int sizeWidth
    property int sizeHeight

    property bool isVisible: false

    opacity: 0
    visible: opacity

    // CenterDialog { id: centeredDialog }

    Rectangle {
        id: coverFrame
        anchors.fill: parent
        anchors.margins: 5
        // color: "yellow"
        color:mainWindow.setPlayList?"lightsteelblue":"black"
        radius: 20
    }

    Label {
        id: artistName
        x: 14; y: 14
        width: parent.width - (parent.width / 4) - x
        maximumLineCount: width
        elide: "ElideLeft"
        color: "#ffffff"
        // muda o title para indicar o modo-playlist
        text: mainWindow.setPlayList?"PLAYLIST":artist
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
                        text:  ("00"+ index ).slice(-2) + "->" + modelData
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
            case Keyboard.Settings.KEY_P:
                console.log("VEIO PRA CA POR QUE PEDIU PRA VOLTAR -> ")
                coverDetailView.hide()
                break;
            case Keyboard.Settings.KEY_SKIP:
                mediaPlayer.playNext();
                if (mainWindow.setPlayList) {
                    console.log("FUCKING ALERT");
                    console.log("essa variavel eh da organizacao das capinhas na memoria ORDER: "+order)
                }
                else {
                    console.log("FALSE - "&&"DISABLED SHIT")
                    console.log(" THE Fucking order: "+order)
                }
                // centeredDialog.visible = true;
                break;
//            case Keyboard.Settings.KEY_BACK:
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
                if(search.number.length < 2 ) {
                    search.number += event.text
                    console.log("event lenght ", search.number.length)
                }

                if(search.number.length === 2) {
                    var index = parseInt(search.number);
                    console.log("mas veio pra antes do setTrack ", search.number.length)
                    console.log("conferir o tamanho da playlist ", playlist.length)
                    if (search.number <= playlist.length) {
                      setTrack(index)
                      console.log("chegou a zerar... ", search.number.length)
                    } else
                        if (search.number === "99") { // nao trata o number como numero...
                            for (var i=0; i<playlist.length; i++) {
                                console.log("ITEM: ", playlist[i])
                                setTrack(i)
                            } // end loop for
                            console.log("CHEGOU A ENTRAR?")
                        } // fim do if
                    console.log("ERA PRA TOCAR TUDO.. ", search.number)
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
            default:
                console.log("FICA APERTANDO AI...", event.key)
                // database.scan(myList1) // --> testei e foi, mandou o array pra lah.
                database.scan([mainWindow.getNameList, index, order, artist, album, imagen])
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
        var songStr = qsTr(playlist[index])
        var artistStr = database.getPathTrack(indexCover, index)
        // var mixTape = database.get2PlayTable(indexCover) --> so funciona com tabela playlist
        console.log("Current indexCover -> ", indexCover)
        /*
          select track.album, album.path, track.name, album.id, artist.name, album.name from TRACK
          inner join ALBUM on track.album = album.id
          inner join ARTIST on artist.id = album.artist
          where album.id = 16;
        */
        // storeArtist(QString name, int genere)
        // storeAlbum(QString name, QString path, QString image, int type)
        // storeTracks(QFileInfoList fileList);
        // mainWindow.fullTime =+ mediaPlayer.getCurrentTime()
        if (mainWindow.setPlayList) {
            if (mainWindow.getNameList == "none") {
                mainWindow.getNameList = database.autoplaylist(mainWindow.getNameList)
                console.log("PlayLIST ID: ", database.storeList(indexCover, artistStr, mainWindow.getNameList, [index, order]))
                // centeredDialog.title = qsTr("PlayLIST ID: "+database.storeList(indexCover, artistStr, mainWindow.getNameList, index)) // strSong
            }
            else {
                // Aqui se executa novamente (era pra ser diferente, mas...)
                // mylist1 = [indexCover, artistStr, mainWindow.getNameList, index, order, artist, album, songStr, imagen]
                console.log("ADICIONANDO MAIS: "+database.storeList(indexCover, artistStr, mainWindow.getNameList, [index, order] ))
                // centeredDialog.title = qsTr("ADICIONANDO MAIS: "+database.storeList(indexCover, artistStr, mainWindow.getNameList, index))
            }
        }
        else {
            console.log("PlayIt! -> time?:", mediaPlayer.getCurrentTime() )
            console.log("songStr ->", songStr )
            console.log("order, artist, album, songStr, imagen, indexCover e index", order, artist, album, songStr, imagen, indexCover, index )
            // console.log("tammanho do mixTape -> ", mixTape)

            /* teste maluco q nao funciona...
            if (getMlist !== "undefined") {
                console.log("tah dentro do PlayList..", getMlist)
                for (var i=0; i<mixTape.length; i++) {
                   getMlist = mixTape[i].split(";")
                   for (var j=0; j<getMlist.length; j++) {
                      console.log("itens ->", getMlist[j], ": ", j)
                   }
                } // end loop for

            } else {
            */

            displayBarItem.playListAdd(order,artist, album, songStr, imagen )
            mediaPlayer.setQueue(database.getPathTrack(indexCover, index), typeCover)
            database.storeHistory(indexCover, songStr, mainWindow.getNameList, index)
        }
        centeredDialog.text = qsTr(artistStr+" "+database.get2PlayTable(indexCover))
        // centeredDialog.text = qsTr(artistStr+" "+getlist[1])
        centeredDialog.visible = true
    }
/***
* Funcao que toca a mixtape toda
* return -> albumId, disc, song, ARTIST.name, position, ALBUM.path, ALBUM.image
*
*/
    function playMixTape(Indx) {
        var mixTape = database.get2PlayTable(Indx)

        if(mixTape.length <= 0)
           return; // se for de um so num vale...

        for (var i=0; i<mixTape.length; i++) {
           getMlist = mixTape[i].split(";")
           for (var j=0; j<getMlist.length; j++) {
              console.log("itens ->", getMlist[j], ": ", j)
           }
           console.log("i, artist, getlist[0], playlist[i] -> ", i, getMlist[1], getMlist[0], playlist[i])
           console.log("0, 1, 2, 3 -> ", getMlist[0], getMlist[1], getMlist[2], getMlist[3])
           displayBarItem.playListAdd(getMlist[4], getMlist[3], getMlist[0], playlist[i], getMlist[5] )
           mediaPlayer.setQueue(getMlist[2], 0)
           // mei que tocou mas sem pregar as covers corretas...
        }
        console.log("executou o loop for...o indx eh Coverdetail ->", Indx )
        console.log("vamu lah mixtape... ", mixTape[0])
        getMlist = 'undefined'
        console.log("and then... erase getlist...", getMlist)
    }
/***
* MIXTAPEPLAY
*
*/
   function runInitialConfiguration() {
        db = LocalStorage.openDatabaseSync(dbId, dbVersion, dbDescription, dbSize);

/***
* DATABASETX -> query.first()
        db.transaction(function(tx) {
            var sql = 'CREATE TABLE IF NOT EXISTS person (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL)';
            tx.executeSql(sql);
        } );
*/

        db.transaction(function(tx) {
            var sql = 'SELECT id, name FROM person';
            var rs = tx.executeSql(sql);
            var ix;
            var myId;
            var myName;
            for (ix = 0; ix < rs.rows.length; ix++) {
               myId = rs.rows.item(ix).id;
               myName = rs.rows.item(ix).name;
               personListview.model.append({
                   id: myId,
                   name: myName });
            }
        } );
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

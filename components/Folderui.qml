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
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Item {
    id: root
    property int maxValueProgressbar: 100
    property int optionTypeFolder: 0

    anchors.fill: parent
    anchors.leftMargin: 20
    //    ScrollView {
    //        id: page
    //        anchors.fill: parent
    //        contentItem: mainColumnLayout
    //    }


    Rectangle {
        id: conet
        anchors.fill: parent
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        color: nativePalette.window

        ColumnLayout {
            id: mainColumnLayout
            anchors.fill: parent
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            Label {
                id: labelTitle
                color: nativePalette.windowText
                text: qsTr("Search file")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 20
                font.bold: true
            }
            RowLayout {
                spacing: 6
                anchors.top: labelTitle.bottom
                anchors.topMargin: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: parent.height * 0.3
                Layout.maximumWidth: parent.width - 10
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight:  true
                    ListView {
                        id: listView1
                        anchors.fill: parent
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 6
                        clip: true
                        model: folderModel
                        highlight: Rectangle { color: nativePalette.highlight; radius: 2 }
                        focus: true

                        Component {
                            id: listDelegate
                            Item  {
                                width: parent.width
                                height: folderInfo.height
                                Text {
                                    id: folderInfo
                                    text: path
                                    font.bold: true
                                    font.pointSize: 10
                                    color: nativePalette.text
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        listView1.currentIndex = index
                                    }
                                }
                            }
                        }
                        delegate: listDelegate
                    }
                }
                ColumnLayout {
                    spacing: 2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    anchors.rightMargin: 20
                    Button {
                        id: button_add
                        Layout.preferredWidth: parent.width
                        text:qsTr("Add")
                        onClicked: {
                            progressBar_update.value = 0
                            fileDialog.visible = true
                        }
                    }
                    Button {
                        id: button_remove
                        text: qsTr("Remove")
                        onClicked: {
                            folderModel.remove(listView1.currentIndex);
                            if(folderModel.count == 0 )
                                button_update.enabled = false
                        }
                    }
                }
            }
            GroupBox {
                title: qsTr("Type folder name")
                Layout.fillWidth: true
                Row {
                    spacing: 8
                    ExclusiveGroup { id: folderName

                    }
                    RadioButton {
                        id: buttonFolderName2
                        text:qsTr("\Artist - Album")
                        antialiasing: true
                        exclusiveGroup: folderName
                        onClicked: {
                            optionTypeFolder = 1;
                        }
                    }

                    RadioButton {
                        id: buttonFolderName1
                        text: qsTr("\\Artist\\Album")
                        exclusiveGroup: folderName
                        checked: true
                        onClicked: {
                            optionTypeFolder = 0;
                        }
                    }
                }
            }
            GroupBox {
                title: qsTr("Type files name")
                Layout.fillWidth: true
                Row {
                    ExclusiveGroup { id: fileName }
                    RadioButton {
                        id: button5
                        text: qsTr("Track Name.mp3")
                        exclusiveGroup: fileName
                    }
                    RadioButton {
                        id: button4
                        text: qsTr("xx Track Name.mp3")
                        exclusiveGroup: fileName
                    }
                    RadioButton {
                        id: button3
                        text: qsTr("xx - TrackName.mp3 (xx=2 digit track number)")
                        exclusiveGroup: fileName
                        checked: true
                    }
                }
            }
            Button {
                id: button_update
                text:qsTr("Update database")
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: false
                onClicked: {
                    enabled = false
                    setPath()
                }
            }
            ProgressBar {
                id: progressBar_update
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.1
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.1
                maximumValue: maxValueProgressbar
                visible: true
            }
        }
    }
    clip: true

    FileDialog {
        id: fileDialog
        title:qsTr("Please choose a folder")
        folder: shortcuts.home
        selectFolder: true
        sidebarVisible: true
        width: Screen.width
        onAccepted: {
            folderModel.append({"path": fileDialog.folder.toString().replace("file:///", ""), "option": false ,"type": optionTypeFolder}) // windows
            button_update.enabled = true
            close();
        }
        onRejected: {
            close();
        }
    }


    ListModel { id: folderModel }

    function setPath() {
        var folderItems = []
        var folderOption = []
        for(var i = 0; i < folderModel.count; i++) {
            folderItems.push(folderModel.get(i).path)
            folderOption.push(false);
        }
        database.setPaths(folderItems, folderOption)
        button_update.enabled = true;
    }

    function progressBarUpdate( count ) {
        progressBar_update.value = count
    }

    Component.onCompleted: { button_update.enabled = false; optionTypeFolder = 0 }
}


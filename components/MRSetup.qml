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
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.0

Item {
    id: root
    property alias uiabout: uiabout
    // property alias uihelp: uihelp
    Rectangle {
        id: main
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        anchors.fill: parent
        color: nativePalette.window
        // color: "red"
        RowLayout {
            anchors.fill: parent
            spacing: 2
            ListView {
                id: listView1
                Layout.fillWidth: true
                Layout.fillHeight:  true
                Layout.maximumWidth: parent.width  * 0.10;
                Layout.maximumHeight: parent.height
                spacing: 10
                delegate: listDelegate
                highlight: Rectangle { color: "lightsteelblue"; radius: 5}
                focus: true
                model: ListModel {
                    ListElement {
                        name: "Folder"
                        path: "qrc:/folder_image"
                    }
                    ListElement {
                        name: "keyboard"
                        path: "qrc:/help_image"
                    }
                    ListElement {
                        name: "Help"
                        path: "qrc:/help_image"
                    }
                    ListElement {
                        name: "About"
                        path: "qrc:/about_image"
                    }
                    ListElement {
                        name: "LookN' Feel"
                        path: "qrc:/look_image"
                    }
                }
            }
            Rectangle {
                Layout.fillWidth:  true
                Layout.fillHeight: true
                color:  nativePalette.window
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 2
                    Rectangle {
                        // anchors.bottom: button_close.top
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color:  nativePalette.window
                        Folderui {
                            id: uifolder
                            // anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            visible: false
                        }
                        Keyboardui {
                            id: uikeyboard
                            anchors.fill: parent
                            visible: false
                        }
                        Helpui {
                            id: uihelp
                            anchors.fill: parent
                            visible: false
                        }
                        Aboutui {
                            id: uiabout
                            anchors.fill: parent
                            visible: false
                        }
                        LookAndFeelui {
                            id: uilook
                            anchors.fill: parent
                            visible: false
                        }

                    }
                    Button {
                        id: button_close
                        text:qsTr("Close")
                        //Layout.fillHeight: true
                        //Layout.fillWidth: true
                        // Layout.bottom: parent.bottom // sempre q bota nao abre.
                        Layout.alignment: Qt.AlignBottom
                        // anchors.bottom: parent.bottom
                        // anchors.right: parent.right
                        // Layout.rightMargin: 20
                        anchors.rightMargin: 20
                        onClicked: {
                            root.visible = false
                        }
                    }
                }
            }
        }
    }

    Component  {
        id: listDelegate

        Rectangle {
            width:  parent.width
            height: width
            color: "transparent"
            // color: "red"
            Image {
                id: pic
                height: parent.height * 0.80
                source: path
                fillMode: Image.PreserveAspectFit
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                color: nativePalette.text
                text: name
                font.bold: true
                font.pointSize: 10
                anchors.horizontalCenter: pic.horizontalCenter
                anchors.top: pic.bottom
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView1.currentIndex = index
                    if(index === 0) {
                        root.state = "Folder"
                    } else if(index === 1) {
                        root.state = "Keyboard"
                    } else if(index === 2) {
                        root.state = "Help"
                    } else if(index === 3) {
                        root.state = "About"
                    } else {
                        root.state = "Look"
                        console.debug("escreve ai")
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "Folder";
            PropertyChanges { target:uifolder; visible: true }
        },
        State {
            name: "Keyboard";
            PropertyChanges { target:uikeyboard; visible: true }
        },
        State {
            name: "Help";
            PropertyChanges { target:uihelp; visible: true }
        },
        State {
            name: "About";
            PropertyChanges { target:uiabout; visible: true }
        },
        State {
            name: "Look"
            PropertyChanges { target:uilook; visible: true }
        }

    ]

    function progressBarMax(value) {
        uifolder.maxValueProgressbar = value
    }

    function progressBarUpdate(value) {
        uifolder.progressBarUpdate(value)
    }
    function progressBarDone() {
        uifolder.progressBarUpdate(uifolder.maxValueProgressbar)
    }

    Component.onCompleted: {
        listView1.currentIndex = 0;
        root.state = "Folder"
        console.log("veio aqui runtime", parent.objectName)
        console.log("[onCompleted]", parent.width)
    }
}

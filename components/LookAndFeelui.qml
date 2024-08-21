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
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "../main"

Item {
    id: root
    property alias groupBox3: groupBox3

    // CenterDialog { id: centeredDialog }

    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        // anchors.fill: parent
        Layout.fillWidth: true
        color: nativePalette.window

        ColumnLayout {
            id: columnLayout1
            spacing: 6
            Layout.fillWidth: true
            Layout.fillHeight: true
            // anchors.fill: parent
            Label {
                id: label1
                color: nativePalette.windowText
                text: qsTr("Look and feel")
                font.bold: true
                font.pixelSize: 24
                Layout.alignment: Qt.AlignHCenter
                // anchors.horizontalCenter: parent.horizontalCenter
            }
            GroupBox {
                id: groupBox1
                title: qsTr("Language")
                Layout.fillWidth: true
                Row {
                    id: row0
                    spacing: 6
                    Label {
                        text: qsTr("Language:")
                    }
                    ComboBox {
                        id: comboBox1
                    //anchors.right: parent.right
                    //anchors.rightMargin: 10
                    }
                }
            }
            GroupBox {
                id: groupBox2
                title: qsTr("Folder Name")
                Layout.fillWidth: true
                Row {
                    id: row4
                    spacing: 2
                    Label {
                        id: label2
                        text: qsTr("Finder look")
                   // anchors.left: parent.left
                   // anchors.leftMargin: 10
                    }
                    ComboBox {
                        id: comboBox2
                   // anchors.right: parent.right
                   // anchors.rightMargin: 10
                    }
                }
            }
            GroupBox {
                id: groupBox3
                title: qsTr("Look")
                Layout.fillWidth: true
                Column {
                    id: column2
                    spacing: 2
                    Row {
                        id: row1
                        Label {
                            id: label3
                            text: qsTr("Background Image:")
                            // anchors.left: parent.left
                            // anchors.leftMargin: 10
                        }
                        spacing: 2
                        Button {
                            id: bg
                            text: qsTr("Change")
                            onClicked: {
                                fileDialog.visible = true
                            }

                        //anchors.left: label3.right
                        //anchors.leftMargin: 25
                        }
                    }
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        title:qsTr("Please choose a folder")
        // folder: shortcuts
        // selectFolder: true
        nameFilters: [ "Image files (*.jpg *.png *.jpeg)", "All files (*)" ]
        // selectedNameFilterExtensions: ["Image files (*.jpg *.png *.jpeg)", "All files (*)"]
        // sidebarVisible: true
        // width: Screen.width
        onAccepted: {
            folderModel.append({"path": fileDialog.folder.toString().replace("file:///", "/"), "option": false ,"type": optionTypeFile})
            close();
        }
        onRejected: {
            close();
        }
        onSelectionAccepted: {
           centeredDialog.title = qsTr("Error!")
           centeredDialog.text = qsTr("You chosed: " + fileDialog.fileUrls )
           centeredDialog.visible = true
        }
    }
}

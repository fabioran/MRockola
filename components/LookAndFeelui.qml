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
import QtQuick.Layouts 1.1

Item {
    id: root

    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        anchors.fill: parent
        color: nativePalette.window

        ColumnLayout {
            id: columnLayout1
            spacing: 6
            anchors.fill: parent
            Label {
                id: label1
                color: nativePalette.windowText
                text: qsTr("Look and feel")
                font.bold: true
                font.pixelSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
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
                            text: qsTr("Background Imge:")
                        //anchors.left: parent.left
                       // anchors.leftMargin: 10
                        }
                        Button {
                            id: button1
                            text: qsTr("Change")
                        //anchors.left: label3.right
                        //anchors.leftMargin: 25
                        }
                    }
                    Row {
                        id: row2
                        visible: false
                        Label {
                            id: label4
                            text: qsTr("Label")
                        }
                        Button {
                            id: button2
                            text: qsTr("Button")
                        }
                    }
                    Row {
                        id: row3
                        visible: false
                        Label {
                            id: label5
                            text: qsTr("Label")
                        }
                        Button {
                            id: button3
                            text: qsTr("Button")
                        }
                    }
                }
            }
        }
    }
}

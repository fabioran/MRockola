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
import QtQuick.Dialogs 1.2

Item {
    id: keyboardView
    property int rowToChange: 0
    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        anchors.fill: parent
        color: nativePalette.window

        ColumnLayout {
            spacing: 6
            anchors.fill: parent
            //Layout.fillHeight: true
            // Layout.fillWidth: true
            Label {
                id: label1
                color: nativePalette.windowText
                text: qsTr("Keyboard config")
                font.bold: true
                font.pixelSize: 20
                Layout.alignment: Qt.AlignHCenter
                // anchors.horizontalCenter: parent.horizontalCenter
            }

            TableView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TableViewColumn {
                    role: "actionStr"
                    title: "Action"
                    width: 200
                    movable: false
                }
                TableViewColumn {
                    role: "actionKey"
                    title: "Key"
                    width: 100
                    movable: false
                }

                model: settings
                itemDelegate: Item {
                    Text {
                        font.pixelSize: 18
                        renderType: Text.NativeRendering
                        text: styleData.value
                    }
                }
                rowDelegate: Rectangle{
                    height: 30
                    color: styleData.selected ? "#C6ECFC" : (styleData.alternate? "#eee" : "#fff")

                }
                headerDelegate: Rectangle {
                     height: textItem.implicitHeight * 2
                     width: textItem.implicitWidth
                     color: "lightsteelblue"
                     Text {
                         id: textItem
                         anchors.fill: parent
                         verticalAlignment: Text.AlignVCenter
                         horizontalAlignment: styleData.textAlignment
                         anchors.leftMargin: 12
                         text: styleData.value
                         elide: Text.ElideRight
                         renderType: Text.NativeRendering
                         font.pixelSize: 16
                     }
                }

                onDoubleClicked: {
                    rowToChange = row
                    inputDialog.open()
                }
            }
        }
    }

    Dialog {
        id: inputDialog
        title: qsTr("action key")
        standardButtons:StandardButton.Ok

        contentItem: Rectangle {
            implicitWidth: 200
            implicitHeight: 100
            Label {
                text: "press any key"
                anchors.centerIn: parent
            }
            Keys.onPressed: {
                console.log("KEY:"+ event.key)
                console.info(event.key)
                settings.changeKey(rowToChange, event.key);
                event.accepted = true
                inputDialog.close()
            }
        }
    }
}

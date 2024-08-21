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
    id: root

    Component.onCompleted: {
        console.log("executou!", parent.objectName)
        console.log("[onCompleted]", parent.width)
        console.log("label1 X", label1.x)
    }

    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        // anchors.fill: parent
        color: nativePalette.window

        ColumnLayout {
            spacing: 6
            // anchors.fill: parent

            Label {
                id: label1
                color: nativePalette.windowText
                text: qsTr("Help me in the Alignment!")
                font.bold: true
                font.pointSize: 20
                // Layout.alignment: Qt.AlignHCenter
                // anchors.horizontalCenter: parent.horizontalCenter
            }
            TextArea {
                id: label2
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pointSize: 13
                textColor: nativePalette.text
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                readOnly: true
                selectByMouse: false
                text: qsTr("Add playlist
Go to folder section, push add button
select your root folder where store all your music and
video, press update database button. Ready!
MRockola organize all for you.

For configuration keyboard, go to keyboard section and
then double click on row where you wish change the key.

Also you choose other option for tune up mrockola.
enjoy!!!")
            }
        }
    }
}

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
import QtQuick.Layouts 1.1

Item {
    id: aboutView
    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        anchors.fill: parent
        color: nativePalette.window

        ColumnLayout {
            spacing: 6
            anchors.fill: parent

            Label {
                id: label1
                color: nativePalette.windowText
                text: qsTr("About")
                font.bold: true
                font.pointSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
            }

            TextArea {
                    id: label2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    textColor: nativePalette.text
                    font.pointSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    readOnly: true
                    selectByMouse: false
                    text: "MRockola 2011-2016 MGHIK TECHNOLOGY\n<https://www.facebook.com/MghikTechnology>\nDesign by Galo Carrillo <http://www.neurodosis.com>
Web page design by Arifur Rahman <http://www.arifur.net/>
some icons are property of NUVOX and BreezeRemix
vinyl image by \"marazmuser\" Artyom Spirin
Visit our web for updates: <http://mrockola.sourceforge.net>

Support: Edgar Lopez <mghikTechnology@gmail.com>
     QA: Isaac Lopez Rodriguez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with This program.  If not, see <http://www.gnu.org/licenses/>.

Version: 3.0b Beta (3 March 2016)"
                }

                Image {
                    id: image1
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/logo-mghik"
                }
        }
    }
}

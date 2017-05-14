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
import QtQuick.Layouts 1.1

Rectangle {
    color: "transparent"
    anchors.fill: parent
    Text {
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter;
            rightMargin: 10
        }
        text: "MRockola V3.0 Beta Copyright 2011-2016"
        font.pointSize: 10
        color: "white"
        wrapMode: Text.WordWrap
    }
}

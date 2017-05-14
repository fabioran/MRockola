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

import QtQuick 2.5

Item {
    id: progressbar
    property int minimum: 0
    property int maximum: 100
    property int value: 0
    property color color: "gray"

    anchors.fill: parent
    anchors.bottomMargin: 6
    anchors.topMargin: 6
    clip: true

    Rectangle {
        id: border
        anchors.fill: parent
        anchors.bottomMargin: 1
        anchors.rightMargin: 1
        color: "transparent"
        border.width: 1
        border.color: parent.color
        radius: 8
    }
    Rectangle {
        id: highlight
        property int widthDest: ( ( progressbar.width * ( value- minimum ) ) / ( maximum - minimum ) - 4 )
        width: highlight.widthDest

        Behavior on width {
            SmoothedAnimation {
                velocity: 1200
            }
        }

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 2
        }
        radius: 8
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ffffff" }
            GradientStop { position: 0.066; color: "#07416c" }
            GradientStop { position: 1.0; color: "#2a75b7" }
        }
    }
}

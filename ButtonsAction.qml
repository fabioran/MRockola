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

Rectangle {
    id: buttonAction
    property alias imageString: icon.source
    property Item rootRect
    property alias blink: anim.running

    signal clicked

    color: "transparent"
    radius: 16

    border.color: "#FFFFFF"; border.width: 2
    width: rootRect.height; height: rootRect.height;

    SequentialAnimation {
        id: anim
        loops: Animation.Infinite

        PropertyAnimation {
            target: buttonAction
            property: "y"
            to: "-10"
            duration: 500

        }
        PropertyAnimation {
            target: buttonAction
            property: "y"
            to: "10"
            duration: 50

        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        BorderImage {
            id: buttonImage
            width: parent.width
            height: parent.height
            Image {
                id: icon
                smooth: true
                width: parent.width
                height: parent.height
                source: imageString
            }
        }
        onClicked: { buttonAction.clicked();}
    }
}

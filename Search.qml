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
    id:  searchView
    property bool isVisible: false
    property string number
    opacity: 0
    visible: opacity
    scale: 0.3

    width: parent.width * 0.20;
    height: parent.width * 0.10;
    Rectangle {
        SystemPalette { id: nativePalette; colorGroup: SystemPalette.Active }
        anchors.fill: parent
        color: "black"
        border.color: "black"

        Text {
            text: number
            anchors.centerIn: parent
            color: "white"
            fontSizeMode: Text.Fit
            minimumPointSize: 14
            font.bold: true
            font.pixelSize: 100
        }

        ParallelAnimation {
            id: showAnimation
            NumberAnimation { target: searchView; property: "opacity"; to: 0.9; duration: 500; easing.type: Easing.InOutQuad }
            NumberAnimation { target: searchView; property: "scale"; to: 1.0; duration: 500; easing.type: Easing.OutBounce }
        }
        ParallelAnimation {
            id: hideAnimation
            NumberAnimation { target: searchView; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InOutQuad }
            NumberAnimation { target: searchView; property: "scale"; to: 0.3; duration: 500; easing.type: Easing.OutBack }
        }
    }

    function show() {
        if(isVisible) {
            searchView.hide()
        } else {
            showAnimation.restart();
            isVisible = true;
        }
    }

    function hide() {
        hideAnimation.restart();
        isVisible = false;
        number = ""
    }
}

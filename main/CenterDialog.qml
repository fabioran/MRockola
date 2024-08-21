// CenteredDialog.qml
import QtQuick.Dialogs 1.2

import QtQuick 2.3
import QtQuick.Controls 1.2

Dialog {
    //this:ApplicationWindow.data

    //x: (this.width - width) / 2
    //y: (this.height - height) / 2

    // focus: true
    // modal: true

    /*
    property alias title: titleText.text
    Text {
        id: titleText
        // text: qsTr("Application")
        verticalAlignment: Text.TopLeft
        horizontalAlignment: Text.AlignTop
        width: Text.width
    }
    */
    property alias text: messageText.text
    Label {
        id: messageText
        width: implicitWidth
        height: implicitHeight
        // verticalAlignment: Qt.AlignBottom
        // horizontalAlignment: Qt.AlignHCenter
        // anchors.fill: parent
    }
    standardButtons: StandardButton.Ok
}

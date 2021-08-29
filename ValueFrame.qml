import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
   id: value_frame
   width: 1000
   height: 1080
   color: "#000000"

   Text {
        id: bpm_val
        x: 62
        y: 322
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr_val
        x: 62
        y: 677
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: temp_val
        x: 62
        y: 859
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: spo2_val
        x: 62
        y: 487
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

}



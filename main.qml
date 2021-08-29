import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 1920
    height: 1080
    visible: true
    color: "#000000"
    visibility: Window.FullScreen

    Text {
        id: bpm
        x: 82
        y: 322
        color: "#ffffff"
        text: qsTr("BPM")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr
        x: 82
        y: 677
        color: "#ffffff"
        text: qsTr("GSR")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: temp
        x: 82
        y: 859
        color: "#ffffff"
        text: qsTr("Temp")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: spo2
        x: 82
        y: 498
        color: "#ffffff"
        text: qsTr("SpO<sub>2</sub>")
        font.pixelSize: 64
        textFormat: Text.RichText
        font.family: "IBM Plex Mono"
    }

    Text {
        id: bpm_val
        x: 329
        y: 322
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr_val
        x: 329
        y: 677
        color: "#ffffff"
        text: gsrUpdater.text
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: temp_val
        x: 329
        y: 859
        color: "#ffffff"
        text: tempUpdater.text
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }


    Text {
        id: spo2_val
        x: 329
        y: 487
        color: "#ffffff"
        text: qsTr("0")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Image {
        id: love_Heart
        x: 139
        y: 46
        width: 159
        height: 234
        source: "images/Love_Heart.svg"
        fillMode: Image.PreserveAspectFit
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

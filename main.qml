import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15

Window {
    width: 1920
    height: 1080
    visible: true
    color: "#000000"
    // visibility: Window.FullScreen

    Text {
        id: bpm
        x: 82
        y: 322
        color: "#ffffff"
        text: qsTr("Pulse")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr
        x: 82
        y: 677
        color: "#ffffff"
        text: qsTr("EDA")
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
        text: bpmUpdater.text
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
        text: qsTr("N/A")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }




    Image {
        id: love_Heart
        x: 704
        y: 331
        width: 513
        height: 418
        source: "images/Love_Heart.svg"
        scale: 1
        fillMode: Image.PreserveAspectFit

        Text {
            id: measurementActiveText
            x: 330
            y: 387
            width: 145
            height: 31
            visible: false
            color: "#ff0000"
            text: btncntrl.statusText
            font.pixelSize: 28
            font.family: "IBM Plex Mono"
        }

        Text {
            id: clickMeText
            x: 104
            y: 163
            color: "#ffffff"
            visible: !btncntrl.clicked
            text: qsTr("Messung starten!")
            font.pixelSize: 34
            font.bold: true
            font.family: "IBM Plex Mono"
        }
    }



    Button {
        id: button
        x: 734
        y: 331
        width: 458
        height: 437
        opacity: 0
        visible: true
        text: qsTr("Button")
        onClicked: btncntrl.setClicked(!btncntrl.clicked)
    }

    Timeline {
        id: thermometerTimeline
        startFrame: timelinecntrl.startFrame
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                running: timelinecntrl.running
                loops: timelinecntrl.loops
                duration: timelinecntrl.duration
                to: 500
                from: timelinecntrl.from
            }
        ]
        endFrame: 500
        enabled: timelinecntrl.enabled

        KeyframeGroup {
            target: love_Heart
            property: "scale"
            Keyframe {
                value: 1.25
                frame: timelinecntrl.frame1
            }

            Keyframe {
                value: 1
                frame: timelinecntrl.frame2
            }
        }
    }

    Image {
        id: thermometer_content
        x: 1327
        y: 79
        source: "images/thermometer_content.png"
        scale: 0.4
        fillMode: Image.PreserveAspectFit
    }

    Rectangle {
        id: rectangle
        x: 1413
        y: 455
        width: 79
        height: thermometercntrl.height
        color: "#000000"
        border.color: "#000000"
    }

    Image {
        id: white_thermometer
        x: 1327
        y: 79
        source: "images/white_thermometer.png"
        scale: 0.4
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: emoji
        x: 1325
        y: 106
        source: emojicntrl.emoji
        // source: "images/emoji5a.png"
        fillMode: Image.PreserveAspectFit
    }

}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}D{i:1}D{i:2}D{i:3}D{i:4}D{i:5}D{i:6}D{i:7}D{i:8}D{i:10}D{i:11}
D{i:9}D{i:12}D{i:13}D{i:18}D{i:19}D{i:20}D{i:21}
}
##^##*/

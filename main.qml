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
        opacity: heartfadecntrl.opacity
        visible: true
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


    Timeline {
        id: heartfadeTimeline
        startFrame: heartfadecntrl.startFrame
        animations: [
            TimelineAnimation {
                id: heartfadeAnimation
                running: heartfadecntrl.running
                loops: heartfadecntrl.loops
                duration: heartfadecntrl.duration
                to: 1000
                from: heartfadecntrl.from
            }
        ]
        endFrame: 1000
        enabled: heartfadecntrl.enabled

        KeyframeGroup {
            target: love_Heart
            property: "opacity"
            Keyframe {
                value: 1
                frame: 0
            }

            Keyframe {
                value: 0.2
                frame: 500
            }

            Keyframe {
                value: 1
                frame: 1000
            }
        }
    }




    Image {
        id: vertical_bar_rainbow
        x: 1672
        y: 131
        width: 75
        height: 872
        source: "images/vertical_bar_rainbow.png"
        fillMode: Image.PreserveAspectFit
    }



    Rectangle {
        id: vertical_bar_black_rectangle
        x: 1675
        y: 186
        width: 70
        height: moodcontrol.height
        visible: true
        color: "#000000"
        border.color: "#000000"
    }



    Image {
        id: vertical_bar_frame
        x: 1672
        y: 170
        width: 75
        height: 794
        source: "images/vertical_bar_frame.png"
        fillMode: Image.PreserveAspectFit
    }


    Image {
        id: emoji
        x: 1646
        y: 100 + moodcontrol.height
        width: 128
        height: 146
        source: emojicntrl.emoji
        // source: "images/emoji5a.png"
        fillMode: Image.PreserveAspectFit
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/

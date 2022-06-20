import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Timeline 1.0
import QtQuick.Controls 2.15

Window {
    x: 11
    width: 1920
    height: 1080
    visible: true
    color: "#000000"
    // visibility: Window.FullScreen

    Text {
        id: bpm
        x: 102
        y: 684
        color: "#ffffff"
        text: qsTr("Pulse")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr
        x: 1472
        y: 959
        color: "#ffffff"
        text: qsTr("Aufregung")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: temp
        x: 980
        y: 959
        color: "#ffffff"
        text: qsTr("Temperatur")
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: bpm_val
        x: 349
        y: 684
        color: "#ffffff"
        text: bpmUpdater.text
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: gsr_val
        x: 306
        y: 822
        visible: false
        color: "#ffffff"
        text: gsrUpdater.text
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }

    Text {
        id: temp_val
        x: 1255
        y: 850
        color: "#ffffff"
        text: tempUpdater.text
        font.pixelSize: 64
        font.family: "IBM Plex Mono"
    }






    Image {
        id: love_Heart
        x: 264
        y: 181
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



    Image {
        id: green_circle
        x: 293
        y: 536
        width: 23
        height: 63
        visible: controllight_control.enabled
        source: "images/green_circle.png"
        fillMode: Image.PreserveAspectFit
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
        x: 1031
        y: 170
        width: 344
        height: 778
        source: "images/thermometer_content.png"
        fillMode: Image.PreserveAspectFit
    }

    Rectangle {
        id: rectangle
        x: 1133
        y: 181
        width: 79
        height: thermometercntrl.height
        color: "#000000"
        border.color: "#000000"
    }

    Image {
        id: white_thermometer
        x: 1114
        y: 56
        width: 176
        height: 1011
        source: "images/white_thermometer.png"
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
        x: 1607
        y: 131
        width: 75
        height: 872
        source: "images/vertical_bar_rainbow.png"
        fillMode: Image.PreserveAspectFit
    }



    Rectangle {
        id: vertical_bar_black_rectangle
        x: 1610
        y: 186
        width: 70
        height: moodcontrol.height
        visible: true
        color: "#000000"
        border.color: "#000000"
    }



    Image {
        id: vertical_bar_frame
        x: 1607
        y: 170
        width: 75
        height: 794
        source: "images/vertical_bar_frame.png"
        fillMode: Image.PreserveAspectFit
    }


    Image {
        id: emoji
        x: 1582
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
    D{i:0;formeditorZoom:2}
}
##^##*/

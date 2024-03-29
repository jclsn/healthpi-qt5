QT += quick multimedia multimediawidgets

CONFIG += c++17
CONFIG += debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DS1820.cpp \
        gsrsensor.cpp \
        main.cpp \
        pulsesensor.cpp \

RESOURCES += qml.qrc

CONFIG += lvlc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fonts/AreaKilometer50.otf \
    fonts/AreaKilometer50.ttf \
    fonts/IBMPlexMono-Bold.ttf \
    fonts/IBMPlexMono-BoldItalic.ttf \
    fonts/IBMPlexMono-ExtraLight.ttf \
    fonts/IBMPlexMono-ExtraLightItalic.ttf \
    fonts/IBMPlexMono-Italic.ttf \
    fonts/IBMPlexMono-Light.ttf \
    fonts/IBMPlexMono-LightItalic.ttf \
    fonts/IBMPlexMono-Medium.ttf \
    fonts/IBMPlexMono-MediumItalic.ttf \
    fonts/IBMPlexMono-Regular.ttf \
    fonts/IBMPlexMono-SemiBold.ttf \
    fonts/IBMPlexMono-SemiBoldItalic.ttf \
    fonts/IBMPlexMono-Thin.ttf \
    fonts/IBMPlexMono-ThinItalic.ttf \
    fonts/Moonglade.ttf \
    fonts/PastiOblique.otf \
    fonts/PastiRegular.otf \
    fonts/Rocks.ttf \
    images/Love_Heart.svg \
    images/dot.png \
    images/heart.png \
    images/heart2.png \
    sounds/single-heartbeat.wav \
    sounds/single-heartbeat2.wav \
    sounds/single-heartbeat3.wav \
    sounds/single-heartbeat4.wav \
    sounds/single-heartbeat5.wav \
    sounds/single-heartbeat6.wav \

HEADERS += \
    ButtonControl.h \
    DS1820.h \
    EmojiControl.h \
    Heartbeat.h \
    TimelineControl.h \
    Updater.h \
    VerticalBarControl.h \
    debug.h \
    global.h \
    gsrsensor.h \
    main.h \
    pulsesensor.h


INSTALLS        = target
target.files    = ../build/HealthPi
#target.path     = /home/pi/CC_folder
target.path     = /usr/bin

arm-linux-gnueabihf-g++{
DEFINES+= RASPI
}


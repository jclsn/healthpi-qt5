QT += quick multimedia multimediawidgets

CONFIG += c++17
CONFIG += debug
env_keep += "XDG_RUNTIME_DIR"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DS1820.cpp \
        Sensor.cpp \
        gsrsensor.cpp \
        main.cpp \
        pulsesensor.cpp \
        wiringPi.c

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
    /home/root/fonts/AreaKilometer50.otf \
    /home/root/fonts/AreaKilometer50.ttf \
    /home/root/fonts/IBMPlexMono-Bold.ttf \
    /home/root/fonts/IBMPlexMono-BoldItalic.ttf \
    /home/root/fonts/IBMPlexMono-ExtraLight.ttf \
    /home/root/fonts/IBMPlexMono-ExtraLightItalic.ttf \
    /home/root/fonts/IBMPlexMono-Italic.ttf \
    /home/root/fonts/IBMPlexMono-Light.ttf \
    /home/root/fonts/IBMPlexMono-LightItalic.ttf \
    /home/root/fonts/IBMPlexMono-Medium.ttf \
    /home/root/fonts/IBMPlexMono-MediumItalic.ttf \
    /home/root/fonts/IBMPlexMono-Regular.ttf \
    /home/root/fonts/IBMPlexMono-SemiBold.ttf \
    /home/root/fonts/IBMPlexMono-SemiBoldItalic.ttf \
    /home/root/fonts/IBMPlexMono-Thin.ttf \
    /home/root/fonts/IBMPlexMono-ThinItalic.ttf \
    /home/root/fonts/Moonglade.ttf \
    /home/root/fonts/PastiOblique.otf \
    /home/root/fonts/PastiRegular.otf \
    /home/root/fonts/Rocks.ttf \
    /home/root/images/Love_Heart.svg \
    /home/root/images/dot.png \
    /home/root/images/heart.png \
    /home/root/images/heart2.png

HEADERS += \
    ButtonControl.h \
    DS1820.h \
    EmojiControl.h \
    Heartbeat.h \
    Sensor.h \
    ThermometerControl.h \
    TimelineControl.h \
    Updater.h \
    debug.h \
    gsrsensor.h \
    main.h \
    pulsesensor.h \
    wiringPi.h


INSTALLS        = target
target.files    = ../build/HealthPi
#target.path     = /home/pi/CC_folder
target.path     = /usr/bin

arm-linux-gnueabihf-g++{
DEFINES+= RASPI
}


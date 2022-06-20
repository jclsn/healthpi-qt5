#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <iostream>
#include <thread>
#include <locale>
#include <string>
#include <sstream>

#include "Updater.h"
#include "DS1820.h"
#include "gsrsensor.h"
#include "pulsesensor.h"
#include "Heartbeat.h"
#include "TimelineControl.h"
#include "VerticalBarControl.h"
#include "EmojiControl.h"
#include "ButtonControl.h"

#include <QtMultimedia/QtMultimedia>


std::vector<Sensor> createSensorObjects();

Updater tempUpdater,
        bpmUpdater,
        gsrUpdater,
        spo2Updater;

Heartbeat heartbeat;
TimelineControl timelinecntrl;
TimelineControl heartfadecntrl;
TimelineControl controllight_control;
VerticalBarControl thermometercntrl;
VerticalBarControl moodcontrol;
EmojiControl emojicntrl;
ButtonControl btncntrl;

DS1820 *p2ds1820;

QTimeLine *timeline;
QMediaPlayer *player;

void updateValues();
void poundHeart();
void fadeHeart(unsigned int times);
void thermometerThread();
unsigned int calcThermometerHeight();

bool sleeping = true;


#endif // MAIN_H




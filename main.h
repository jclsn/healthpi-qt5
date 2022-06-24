#ifndef MAIN_H
#define MAIN_H

#include "ButtonControl.h"
#include "DS1820.h"
#include "EmojiControl.h"
#include "Heartbeat.h"
#include "TimelineControl.h"
#include "Updater.h"
#include "VerticalBarControl.h"
#include "global.h"
#include "gsrsensor.h"

#include <QtMultimedia/QtMultimedia>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


std::vector<Sensor> createSensorObjects();

Updater tempUpdater, bpmUpdater, gsrUpdater, spo2Updater;

Heartbeat heartbeat;
TimelineControl timelinecntrl;
TimelineControl heartfadecntrl;
TimelineControl controllight_control;
VerticalBarControl thermometercntrl;
VerticalBarControl moodcontrol;
EmojiControl emojicntrl;
ButtonControl btncntrl;

DS1820* p2ds1820;

QTimeLine* timeline;
QMediaPlayer* player;

void updateValues();
void poundHeart();
void fadeHeart(unsigned int times);
void thermometerThread();
unsigned int calcThermometerHeight();

bool sleeping = true;


#endif    // MAIN_H


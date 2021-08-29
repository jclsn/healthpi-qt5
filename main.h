#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <iostream>
#include <thread>

#include "Updater.h"
#include "DS1820.h"
#include "gsrsensor.h"

std::vector<Sensor> createSensorObjects();

Updater tempUpdater,
        bpmUpdater,
        gsrUpdater,
        spo2Updater;

void updateValues();
#endif // MAIN_H




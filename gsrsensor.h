#ifndef GSRSENSOR_H
#define GSRSENSOR_H

#include <iostream>
#include <string>
#include <algorithm>
#include <Sensor.h>


extern "C"{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <wiringPi.h>
#include <errno.h>
}

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
     } while (0)




class GSRSensor : Sensor
{
    int threshold = 0;
    int sensorValue;

public:
    GSRSensor();

    unsigned int getGSRRaw();
    float getGSRVoltage();
    unsigned int getHumanResistance();
    void GSRThread();
    void setupGSR();
};

#endif // GSRSENSOR_H

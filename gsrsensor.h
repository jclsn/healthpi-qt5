#ifndef GSRSENSOR_H
#define GSRSENSOR_H

#include <iostream>
#include <string>
#include <algorithm>
#include <Sensor.h>
#include <deque>

extern "C"{
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>
}

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
     } while (0)




class GSRSensor : Sensor
{
    int threshold;
    float sensorValue;
    float voltage;
    float avg_voltage;
    float resistance;
    float conductance;
    std::deque <float> a_sensorValues{};

public:
    GSRSensor();

    int readSensor();
    void updateHumanResistance();
    void updateHumanConductance();
    float getGSRVoltage();
    float getGSRAvgVoltage();
    std::string getGSRAvgVoltageString();
    std::string getHumanResistance();
    std::string getHumanConductance();
};

#endif // GSRSENSOR_H

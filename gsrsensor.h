#ifndef GSRSENSOR_H
#define GSRSENSOR_H

#include <Sensor.h>
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>

extern "C" {
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wiringPi.h>
}

#define errExit(msg)        \
	do {                    \
		perror(msg);        \
		exit(EXIT_FAILURE); \
	} while (0)

class GSRSensor : Sensor {
	int threshold{};
	float sensorValue{};
	float voltage{};
	float avg_voltage{};
	float resistance{};
	float conductance{};
	std::deque<float> a_sensorValues{};

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

#endif    // GSRSENSOR_H

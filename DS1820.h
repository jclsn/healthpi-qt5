/*
 * =====================================================================================
 *
 *       Filename:  ds1820.h
 *
 *    Description:  Function to read the temperature value from the DS1820 integraton
 *                  provided by the Linux kernel
 *
 *        Version:  1.0
 *        Created:  12.08.2021 12:05:10 CEST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jan Claussen, jan.claussen10@web.de
 *
 * =====================================================================================
 */

// #define DEBUG 1

#include "Sensor.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {
#include <dirent.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
}

#define errExit(msg)        \
	do {                    \
		perror(msg);        \
		exit(EXIT_FAILURE); \
	} while (0)

class DS1820 : Sensor {
    std::string raw_temperature;

public:
	void read_sensor();
	std::string get_formatted_temperature();
	unsigned int get_thermometer_height();
};

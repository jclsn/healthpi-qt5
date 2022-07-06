/*
 * =====================================================================================
 *
 *       Filename:  ds1820.h
 *
 *    Description:  Function to read the temperature value from sysfs
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


#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

class DS1820 {
    std::string raw_temperature;

public:
	void read_sensor();
	std::string get_formatted_temperature();
	unsigned int get_thermometer_height();
};

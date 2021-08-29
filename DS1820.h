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

#include <iostream>
#include <string>
#include <algorithm>

#include "Sensor.h"

extern "C"{
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
}

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
    } while (0) 


class DS1820 : Sensor
{

public:
    std::string getTemp();
};

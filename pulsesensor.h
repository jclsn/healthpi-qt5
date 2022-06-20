#ifndef PULSESENSOR_H
#define PULSESENSOR_H

#include <iostream>
#include <string>
#include <algorithm>

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
#include "wiringPi.h"
#include <errno.h>
}

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
     } while (0)

// PULSE SENSOR LEDS
#define BLINK_LED 0
//#define FADE_LED      // NEEDS THIS FEATURE ADDED

// MCP3004/8 SETTINGS
#define BASE 100
#define SPI_CHAN 0
#define PULSE_SIGNAL_MAX 1023
#define PULSE_SIGNAL_MIN 0
#define PULSE_SIGNAL_IDLE 512
#define JITTER_SIGNAL_IDLE 0
#define DATA_POINTS 100

int getBPM();
int plotBPMData();
extern volatile bool pulse;
extern volatile bool sensor_is_reading;

#endif // PULSESENSOR_H

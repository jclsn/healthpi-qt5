#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
}

// PULSE SENSOR LEDS
#define BLINK_LED 0
//#define FADE_LED      // NEEDS THIS FEATURE ADDED

// MCP3004/8 SETTINGS
#define BASE               100
#define SPI_CHAN           0
#define PULSE_SIGNAL_MAX   1023
#define PULSE_SIGNAL_MIN   0
#define PULSE_SIGNAL_IDLE  512
#define JITTER_SIGNAL_IDLE 0
#define DATA_POINTS        100


void clock_init();

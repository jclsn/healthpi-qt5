
#pragma once

#include <chrono>
int get_bpm();
int read_bpm_thread();

extern volatile bool pulse;
extern volatile bool sensor_is_reading;

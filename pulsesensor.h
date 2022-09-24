#pragma once

#include "Heartbeat.h"
#include "debug.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ratio>
#include <string>
#include <thread>

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

class Pulsesensor {

public:
	Pulsesensor() :
      sensor_is_reading{false},
      last_time{static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::microseconds>(start - now).count())},
      first_beat{true},
      second_beat{false},
      beat_counter{},
      threshold_setting{},
      last_beat_time{},
      fade_level{},
      threshold{default_threshold},
      peak{default_peak},
      trough{default_trough},
      qs{},
      rate{},
      ibi{420},
      ibi_threefifth{ibi * 3 / 5},
      pulse{false},
      amp{100},
      bpm{},
      start{std::chrono::high_resolution_clock::now()},
      now{start}
    {
    }

	int read_bpm_thread();
    int get_bpm() const;
    volatile bool sensor_is_reading;    // set to 1 while Signal > Threshold

    void start_thread();

private:
    int get_raw_signal() const;
    void get_pulse();
    void stopTimer(void);
	void init_pulsesensor_vars(void);
	void initJitterVariables(void);

    /* VARIABLES USED TO DETERMINE SAMPLE JITTER & TIME OUT */

    volatile unsigned int event_counter, this_time, last_time, elapsed_time;
    volatile int jitter_sum, jitter;
    unsigned int timeout_start, data_request_start, m;
    volatile unsigned int sample_counter;
    volatile int pulse_signal;

    /* DEFAULTS */

    const int default_peak = 550;
    const int default_trough = 550;
    const int default_threshold = 550;


    /* VARIABLES USED TO DETERMINE BPM */

    volatile bool first_beat;     // set these to avoid noise
    volatile bool second_beat;    // when we get the heartbeat back
    volatile int beat_counter;
    volatile int threshold_setting, last_beat_time, fade_level;
    volatile int threshold;    // SHOULD BE ADJUSTABLE ON COMMAND LINE
    volatile int peak;         // set P default
    volatile int trough;       // set T default
    volatile int qs;
    volatile int rate[10];
    volatile int ibi;               // 600ms per beat = 100 Beats Per Minute (BPM) (inter-beat interval)
    volatile int ibi_threefifth;    // 600ms per beat = 100 Beats Per Minute (BPM) (inter-beat interval)
    volatile bool pulse;            // set to 1 while Signal > Threshold
    volatile int amp;               // beat amplitude 1/10 of input range.
    volatile std::atomic<int> bpm;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> now;
    std::chrono::microseconds duration{};
};

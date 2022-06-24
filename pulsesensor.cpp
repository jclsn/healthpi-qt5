#include "pulsesensor.h"

#include "Heartbeat.h"
#include "debug.h"

/* VARIABLES USED TO DETERMINE SAMPLE JITTER & TIME OUT */

volatile unsigned int event_counter, this_time, last_time, elapsed_time;
volatile int jitter_sum, jitter;
unsigned int timeout_start, data_request_start, m;

/* DEFAULTS */

const int default_peak = 550;
const int default_trough = 550;
const int default_threshold = 550;


/* VARIABLES USED TO DETERMINE BPM */

volatile unsigned int sample_counter;
volatile bool first_beat = true;      // set these to avoid noise
volatile bool second_beat = false;    // when we get the heartbeat back
volatile int beat_counter = 0;
volatile int pulse_signal;
volatile int threshold_setting, last_beat_time, fade_level;
volatile int threshold = default_threshold;    // SHOULD BE ADJUSTABLE ON COMMAND LINE
volatile int peak = default_peak;              // set P default
volatile int trough = default_trough;          // set T default
volatile int qs = 0;
volatile int rate[10];
volatile int ibi = 600;                       // 600ms per beat = 100 Beats Per Minute (BPM) (inter-beat interval)
volatile int ibi_threefifth = ibi * 3 / 5;    // 600ms per beat = 100 Beats Per Minute (BPM) (inter-beat interval)
volatile bool pulse = false;                  // set to 1 while Signal > Threshold
volatile int amp = 100;                       // beat amplitude 1/10 of input range.
volatile int bpm = 0;
volatile bool sensor_is_reading = false;    // set to 1 while Signal > Threshold

/* FUNCTION PROTOTYPES */

void get_pulse();
void stopTimer(void);
void init_pulsesensor_vars(void);
void initJitterVariables(void);

int get_bpm()
{
	return bpm;
}

int get_raw_signal()
{
	std::filesystem::path pulsesensor_path{"/sys/bus/spi/devices/spi0.0/iio:device0/in_voltage0_raw"};
	std::ifstream pulsesensor_handle{pulsesensor_path};

	std::string voltage{};

	if (pulsesensor_handle.is_open())
		pulsesensor_handle >> voltage;
	else {
		std::cout << "No pulsesensor found!" << std::endl;
		return 0;
	}

	pulsesensor_handle.close();

	return std::stoi(voltage);
}

void init_pulsesensor_vars(void)
{
	for (int i = 0; i < 10; ++i)
		rate[i] = 0;

	qs = 0;
	bpm = 0;
	ibi = 420;    // 420 per beat = 70 Beats Per Minute (BPM)
	pulse = false;
	sensor_is_reading = false;
	sample_counter = 0;
	last_beat_time = 0;
	peak = default_peak;              // Peak at 1/2 the input range of 0..1023
	trough = default_trough;          // Trough at 1/2 the input range.
	threshold_setting = 550;          // Used to seed and reset the thresh variable
	threshold = default_threshold;    // Threshold a little above the trough
	amp = 100;                        // Beat amplitude 1/10 of input range.
	first_beat = true;                // Looking for the first beat
	second_beat = false;              // Not yet looking for the second beat in a row
	last_time = micros();
}

void get_pulse()
{    // int sig_num){

	this_time = micros();    // Time passed since the program has started

	pulse_signal = get_raw_signal();    // Get the raw sensor data

	// pulse_signal = -400 + (pulse_signal * pulse_signal) / 600;      // Adjust the sensor graph to fit nicely into GNU
	// plot
	//  Also required for the pulse to be detected
	pulse_signal = pulse_signal - 550;
	// pulse_signal = pulse_signal * 2 / 3;
	// pulse_signal = pulse_signal / 2;

	elapsed_time = this_time - last_time;    // Calculate the time between two measurements
	last_time = this_time;                   // Update the time of the last measurement

	jitter = elapsed_time - 2000;
	jitter_sum += jitter;

	sample_counter += 2;                        // Keep track of the time in ms with this variable
	int N = sample_counter - last_beat_time;    // Monitor the time since the last beat to avoid noise


	ibi_threefifth = (ibi / 5) * 3;

	/*  Find the peak and trough of the pulse wave */

	if (pulse_signal < threshold &&
	    N > ibi_threefifth) {    // Avoid dichroitic noise by waiting 3/5 of last IBI (inter-beat interval)
		if (pulse_signal < trough)
			trough = pulse_signal;    // Keep track of lowest point in pulse wave
	}

	if (pulse_signal > threshold && pulse_signal > peak) {    // Thresh condition helps avoid noise
		peak = pulse_signal;
	}    // Keep track of highest point in pulse wave

	/*
	 *  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
	 *  pulse_signal surges up in value every time there is a pulse
	 */

	if (N > 250) {    // Avoid high frequency noise

		if ((pulse_signal > threshold) && (pulse == 0) && (N > ibi_threefifth)) {
			pulse = true;                             // Set the Pulse flag when we think there is a pulse
			ibi = sample_counter - last_beat_time;    // meaSure time between beats in ms
			last_beat_time = sample_counter;          // keeP track of time for next pulse

			beat_counter++;

			if (beat_counter > 3) {
				sensor_is_reading = true;

				if (!sleeping)
					enableHeart();
			}

			if (!sleeping)
				poundHeart();

			if (second_beat) {                  // if this is the second beat, if secondBeat == TRUE
				second_beat = false;            // Clear secondBeat flag
				                                //
				for (int i = 0; i <= 9; i++)    // Seed the running total to get a realisitic BPM at startup
					rate[i] = ibi;
			}

			if (first_beat) {          // if It's the first time we found a beat, if firstBeat == TRUE
				first_beat = false;    // Clear firstBeat flag
				second_beat = true;    // Set the second beat flag
				// IBI value is unreliable so discard it
				return;
			}

			/* keep a running total of the last 10 IBI values */

			int runningTotal = 0;    // Clear the runningTotal variable

			for (int i = 0; i <= 8; i++) {    // Shift data in the rate array
				rate[i] = rate[i + 1];        // and drop the oldest IBI value
				runningTotal += rate[i];      // add up the 9 oldest IBI values
			}

			rate[9] = ibi;                      // Add the latest IBI to the rate array
			runningTotal += rate[9];            // Add the latest IBI to runningTotal
			runningTotal /= 10;                 // Average the last 10 IBI values
			bpm = -5 + 60000 / runningTotal;    // How many beats can fit into a minute? that's BPM!
			if (bpm < 0)
				bpm = 0;
			qs = 1;    // Set Quantified Self flag (we detected a beat)
			// fadeLevel = MAX_FADE_LEVEL;             	// If we're fading, re-light that LED.
		}
	}

	if (pulse_signal < threshold && pulse == 1) {    // When the values are going down, the beat is over
		pulse = false;                               // Reset the Pulse flag so we can do it again
		amp = peak - trough;                         // Get amplitude of the pulse wave
		threshold = amp / 2 + trough;                // Set thresh at 50% of the amplitude
		peak = threshold;                            // Reset these for next time
		trough = threshold;
	}

	if (N > 2500) {    // If 2.5 seconds go by without a beat
		beat_counter = 0;
		sensor_is_reading = false;
		disableHeart();
		threshold = threshold_setting;      // Set thresh default
		peak = default_peak;                // Set P default
		trough = default_trough;            // Set T default
		last_beat_time = sample_counter;    // Bring the lastBeatTime up to date
		first_beat = true;                  // Set these to avoid noise
		second_beat = false;                // When we get the heartbeat back
		qs = 0;
		bpm = 0;
		ibi = 600;    // 600ms per beat = 100 Beats Per Minute (BPM)
		pulse = false;
		amp = 100;    // Beat amplitude 1/10 of input range.

		// std::cout << "Pulsesensor doesn't receive anything!" << std::endl;
	}

#ifdef DEBUG_PULSESENSOR
	std::cout << "pulse: " << pulse << "    sample: " << sample_counter << "    BPM: " << bpm << "    IBI: " << ibi
	          << "    3/5 IBI: " << ibi_threefifth << "    jitter: " << jitter << "    peak: " << peak
	          << "    trough: " << trough << "    threshold: " << threshold << "    signal: " << pulse_signal
	          << std::endl;
#endif
}

int read_bpm_thread()
{
	while (1) {
		delayMicroseconds(1800);    // DELAY TIME NEEDS TO BE ATOMATED
		get_pulse();
	}

	return 0;
}


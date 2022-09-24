#include "pulsesensor.h"

int Pulsesensor::get_raw_signal() const
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

void Pulsesensor::get_pulse()
{    // int sig_num){

	/* this_time = micros();    // Time passed since the program has started */
	now = std::chrono::high_resolution_clock::now();
	this_time = std::chrono::duration_cast<std::chrono::microseconds>(start - now).count();

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

int Pulsesensor::read_bpm_thread()
{

    while (1) {
		std::this_thread::sleep_for(std::chrono::microseconds{1800});
		get_pulse();
	}

	return 0;
}

int Pulsesensor::get_bpm() const
{
    return bpm;
}


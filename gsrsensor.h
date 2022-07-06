#pragma once

#include <algorithm>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class GSRSensor {
	int threshold{};
	float sensor_value{};
	float voltage{};
	float avg_voltage{};
	float resistance{};
	float conductance{};
	std::deque<float> a_sensor_values{};

public:
	GSRSensor();

	void read_sensor();
	void update_sample_list();
	void update_resistance();
	void update_conductance();
	float get_voltage();
	float get_avg_voltage();
	std::string get_avg_voltage_string();
	std::string get_resistance_string();
	std::string get_conductance_string();
};


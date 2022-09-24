#include "DS1820.h"

void DS1820::read_sensor()
{
	std::filesystem::path w1_devices_path{"/sys/bus/w1/devices/"};

	std::string prefix{"28-"};
	std::filesystem::path suffix{"temperature"};
	std::filesystem::path ds1820_path{};

	for (std::filesystem::path const& device_path : std::filesystem::directory_iterator{w1_devices_path}) {
		std::string device_name{device_path.filename().c_str()};

		if (device_name.substr(0, prefix.length()) == prefix)
			ds1820_path = device_path / suffix;
	}

	std::ifstream ds1820_handle{ds1820_path};

	if (!ds1820_handle.is_open())
		return;

	ds1820_handle >> raw_temperature;
}

/* Construct a std::string from the C-string */

std::string DS1820::get_formatted_temperature()
{

	std::string temperature = raw_temperature;

	if (temperature == "N/A")
		return temperature;

	try {
		if (!temperature.empty()) {
			temperature.erase(std::remove(temperature.begin(), temperature.end(), '\n'), temperature.end());
			temperature.insert(2, ".");
			temperature.replace(4, 2, " °C");
		}

	} catch (std::exception e) {
	}

	return temperature;
}

/* Function to calculate the height of the thermomether graphic */

unsigned int DS1820::get_thermometer_height()
{

	std::string tempstring = raw_temperature;

	float temp_float;

	try {
		temp_float = std::stof(tempstring.insert(2, "."));

		if (temp_float < 29.0)
			temp_float = 29.0;

		// temp_float = 29.5;

		unsigned int height = ((unsigned int) fabs(650 - ((temp_float - 29.0) * 54.16)));

#ifdef DEBUG_TEMP_SENSOR
		std::cout << "height = " << height << ", temp = " << temp_float << ", temp_str = " << tempstring << std::endl;
#endif

		return height;

	} catch (std::exception e) {
	}

	return 0;
}

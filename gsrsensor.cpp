#include "gsrsensor.h"
#include <string>

GSRSensor::GSRSensor() {}

void GSRSensor::read_sensor()
{

	std::filesystem::path gsr_sensor_path{"/sys/bus/spi/devices/spi0.0/iio:device0/in_voltage3_raw"};
	std::ifstream gsr_sensor_handle{gsr_sensor_path};

	std::string sensorValueString{};

	gsr_sensor_handle >>  sensorValueString;

	sensor_value = stoi(sensorValueString) * 1.5;

	/* Convert sensor value to voltage */

	voltage = (sensor_value * 5. / 1024.0);
}

void GSRSensor::update_sample_list(){

	/* Place new voltage value in the array */
	read_sensor();

	a_sensor_values.emplace_back(voltage);

	if (a_sensor_values.size() > 10)
		a_sensor_values.pop_front();
}

float GSRSensor::get_avg_voltage()
{
	float sum{};
	for (float value : a_sensor_values)
		sum += value;

	return sum / a_sensor_values.size();
}

std::string GSRSensor::get_avg_voltage_string()
{

	std::string voltage_string = std::to_string(get_avg_voltage());

	if (!voltage_string.empty()) {
		try {
			voltage_string.replace(voltage_string.find(","), 1, ".");
			voltage_string.append(" V");
		} catch (std::exception e) {
		}
	}

	std::cout << voltage_string << std::endl;

	return voltage_string;
}

void GSRSensor::update_resistance()
{
	resistance = ((1024 + 2 * get_avg_voltage()) * 10000) / (512 - get_avg_voltage());
}

std::string GSRSensor::get_resistance_string()
{
	char resistance_c[20]{};
	sprintf(resistance_c, "%f Ω", resistance);
	return std::string(resistance_c);
}

void GSRSensor::update_conductance()
{
	conductance = (1.0 / resistance) * 1000;
}

std::string GSRSensor::get_conductance_string()
{
	char conductance_c[20]{};
	sprintf(conductance_c, "%f", (1.0 / resistance) * 1000);
	return std::string(conductance_c);
}

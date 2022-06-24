#include "gsrsensor.h"

GSRSensor::GSRSensor() {}

int GSRSensor::readSensor()
{
	int fd{};
	char path[60];
	int len = 20;
	ssize_t ret;

	std::string sensorValueString;

	char* p2buf = (char*) calloc(len, sizeof(char));
	char* buf = p2buf;

	for (int i = 0; i < 10; ++i) {
		sprintf(path, "/sys/bus/spi/devices/spi0.0/iio:device%d/in_voltage3_raw", i);
		fd = open(path, O_RDONLY);
		if (fd > 0)
			break;
	}

	if (fd == -1) {
		std::cout << "Couldn't access GSR-sensor." << std::endl;
		return -1;
	}


	if (buf == NULL)
		perror("calloc");

	while (len != 0 && (ret = read(fd, buf, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			perror("read");
			break;
		}

		len -= ret;
		buf += ret;
	}

	if (close(fd) == -1)
		perror("close");

	sensorValueString = std::string(p2buf);

	if (p2buf)
		free(p2buf);

	sensorValue = stoi(sensorValueString) * 1.5;

	/* Convert sensor value to voltage */

	voltage = (sensorValue * 5. / 1024.0);

	/* Place new voltage value in the array */

	a_sensorValues.emplace_back(voltage);

	if (a_sensorValues.size() > 10)
		a_sensorValues.pop_front();

	// std::cout << sensorValue << " " << voltage << std::endl;

	return 0;
}

float GSRSensor::getGSRAvgVoltage()
{
	float sum{};
	for (float value : a_sensorValues)
		sum += value;

	return sum / a_sensorValues.size();
}

std::string GSRSensor::getGSRAvgVoltageString()
{

	char voltage_c[20];
	std::string voltage_string{};
	sprintf(voltage_c, "%.2f", getGSRAvgVoltage());
	voltage_string = std::string(voltage_c);

	if (!voltage_string.empty()) {
		try {
			voltage_string.replace(voltage_string.find(","), 1, ".");
			voltage_string.append(" V");
		} catch (std::exception e) {
		}
	}

	return voltage_string;
}

void GSRSensor::updateHumanResistance()
{
	resistance = ((1024 + 2 * getGSRAvgVoltage()) * 10000) / (512 - getGSRAvgVoltage());
}

std::string GSRSensor::getHumanResistance()
{
	char resistance_c[20]{};
	sprintf(resistance_c, "%f Ω", resistance);
	return std::string(resistance_c);
}

void GSRSensor::updateHumanConductance()
{
	conductance = (1.0 / resistance) * 1000;
}

std::string GSRSensor::getHumanConductance()
{
	char conductance_c[20]{};
	sprintf(conductance_c, "%f", (1.0 / resistance) * 1000);
	return std::string(conductance_c);
}

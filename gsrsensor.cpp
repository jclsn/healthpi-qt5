#include "gsrsensor.h"

GSRSensor::GSRSensor()
{

}

unsigned int GSRSensor::getGSRRaw()
{
    int fd = open("/sys/bus/spi/devices/spi0.0/iio:device0/in_voltage3_raw", O_RDONLY);

    if (fd == -1) {

        fd = open("/sys/bus/spi/devices/spi0.0/iio:device1/in_voltage3_raw", O_RDONLY);

        if (fd == -1) {
            std::cout << "Couldn't open pulse sensor" << std::endl;
            return -1;
        }
    }

    char * voltage_c = (char *) calloc(7, sizeof(char));

    if( voltage_c == NULL )
        errExit("calloc");

    if( fd == -1 )
        std::cout << "Couldn't access skinMoistureSensor voltage sensor." << std::endl;

    if( read(fd, voltage_c, 7) == -1 )
        errExit("read");

    if( close(fd) == -1 )
        errExit("close");

    std::string voltage{voltage_c};
    free(voltage_c);

    return stoi(voltage);

}

float GSRSensor::getGSRVoltage()
{
    return (getGSRRaw() * 5.0 / 1024.0);
}

unsigned int GSRSensor::getHumanResistance()
{
    unsigned int GSR = getGSRRaw();

    return ((1024 + 2 * GSR) * 10000) / (512 - GSR) / 2;

}

void GSRSensor::GSRThread()
{

    for(;;) {

        int temp;
        sensorValue=getGSRRaw();

        std::cout << "sensorValue = " << sensorValue << "\r";
        fflush(stdout);
        temp = threshold - sensorValue;

        if(abs(temp) > 50)
        {
            sensorValue=getGSRRaw();

            temp = threshold - sensorValue;
            if(abs(temp) > 50){
                /* std::cout << "                                BUZZ!!!" << "\r"; */
                /* fflush(stdout); */
                usleep(3000);
            }
        }
    }


}

void GSRSensor::setupGSR()
{
    long sum=0;
    usleep(1000);

    for(int i=0;i<500;i++)
    {
        sensorValue = getGSRRaw();
        sum += sensorValue;
        usleep(5);
    }

    threshold = sum/500;

}

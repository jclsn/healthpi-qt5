

/* Abstract sensor class to inherit from */

#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{

public:
    Sensor();
    int init();

protected:
    int value;
};

#endif // SENSOR_H

#include "DS1820.h"


std::string DS1820::getTemp()
{
        int fd_ds1820_temp = open("/sys/bus/w1/devices/28-3c01d075e10b/temperature", O_RDONLY);

        char * temp_c = (char *) calloc(7, sizeof(char));

        if( temp_c == NULL )
            errExit("calloc");

        if( fd_ds1820_temp == -1 )
            std::cout << "Couldn't access DS1820 temperature sensor." << std::endl;

        if( read(fd_ds1820_temp, temp_c, 7) == -1 )
            errExit("read");

        if( close(fd_ds1820_temp) == -1 )
            errExit("close");

        std::string temp{temp_c};

        temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
        temp.insert(2, ".");
        temp.replace(4, 2, " °C");

        free(temp_c);

        return temp;
}

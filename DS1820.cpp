#include "DS1820.h"

void DS1820::readSensor()
{
        std::string sensor_id{};
        bool sensorFound = false;
        int fd{};

        /* Try to find the sensor in the one-wire device tree */

        DIR *d{};
        struct dirent *dir{};

        if( (d = opendir("/sys/bus/w1/devices/")) == NULL) {
            perror("opendir");
            rawTemp = "N/A";
        }

#ifdef DEBUG
        std::cout << "\nScanning for temperature sensors..." << std::endl;
#endif
        while( (dir = readdir(d)) != NULL) {
#ifdef DEBUG
            std::cout << dir->d_name << std::endl;
#endif
            if( strstr(dir->d_name, "28-") != NULL ) {
                sensor_id = dir->d_name;
                sensorFound = true;
                closedir(d);
                break;
            }
        }

        if(sensorFound == false) {
            closedir(d);
            rawTemp = "N/A";
        }

        std::string str_path = "/sys/bus/w1/devices/" + sensor_id + "/temperature";

#ifdef DEBUG
        std::cout << str_path << std::endl;
#endif

        const char * path = str_path.c_str();

        if( (fd = open(path, O_RDONLY) ) < 0) {
            std::cout << "Couldn't access DS1820 temperature sensor in device tree" << std::endl;
            rawTemp = "N/A";
        }

        /* Seek to the end of the file to determine its length */ 

        off_t len{};
        ssize_t ret{};

        if( (len = lseek(fd, 0, SEEK_END)) == (off_t) -1)
            errExit("lseek");

        /* Seek back to the beginning */ 

        if( (ret = lseek(fd, 0, SEEK_SET)) == (off_t) -1)
            errExit("lseek");

        /* Allocate memory for reading and keep a pointer to the beginning for closing */

        char *p2buf = (char *) calloc(len, sizeof(char));
        char *buf = p2buf;

        if( buf == NULL )
            errExit("calloc");

        /* 
         *  Read the file: 
         *  If reading was incomplete update len and read again.
         */

        while (len != 0 && (ret = read (fd, buf, len)) != 0) {
            if (ret == -1) {
                if (errno == EINTR)
                    continue;
                perror ("read");
                break;
            }

            len -= ret;
            buf += ret;
        }

        if( close(fd) == -1 ) {
            perror("close");
            return;
        }

        std::string temp{p2buf};

        if(p2buf) {
            free(p2buf);
        }

        if(temp.empty())
            return;

        /* // Add some degrees if needed */
        /* int degrees = 3; */
        /* int second_decimal = temp.at(1); */

        /* if(second_decimal < 58 && second_decimal > 47) { */
        /*     if(second_decimal < (58 - degrees) ) */
        /*         temp.at(1) += degrees; */
        /*     else { */
        /*         temp.at(1) -= 10 - degrees; */
        /*         if(temp.at(0) != 9) */
        /*             temp.at(0) += 1; */
        /*         else */
        /*             temp.at(0) -= 9; */
        /*     } */
        /* } */

        if(!temp.empty())
            rawTemp = temp;
}

/* Construct a std::string from the C-string */

std::string DS1820::getTempString() {

    std::string temp = rawTemp;

    if(temp == "N/A")
        return temp;

    try {
        if(!temp.empty()) {
            temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
            temp.insert(2, ".");
            temp.replace(4, 2, " °C");
        }

    }  catch (std::exception e) {

    }

    return temp;
}

/* Function to calculate the height of the thermomether graphic */

unsigned int DS1820::getThermometerHeight() {

        std::string tempstring = rawTemp;

        float temp_float;

        try {
            temp_float= std::stof(tempstring.insert(2, "."));

            if(temp_float < 29.0)
                temp_float = 29.0;

            // temp_float = 29.5;

            unsigned int height = ((unsigned int) fabs(650 - ((temp_float - 29.0) * 54.16)) );
             std::cout << "height = " << height << ", temp = " << temp_float << ", temp_str = " << tempstring <<  std::endl;

            return height;

        }  catch (std::exception e) {}

    return 0;
}

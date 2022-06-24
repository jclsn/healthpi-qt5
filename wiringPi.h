
#ifndef WIRINGPI_H
#define WIRINGPI_H

#include <stdint.h>
#include <sys/time.h>
#include <time.h>


void initialiseEpoch(void);
void delay(unsigned int howLong);
void delayMicrosecondsHard(unsigned int howLong);
void delayMicroseconds(unsigned int howLong);
unsigned int millis(void);
unsigned int micros(void);


#endif

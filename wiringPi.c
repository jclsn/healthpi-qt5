#include "wiringPi.h"

static uint64_t epochMilli, epochMicro;

void initialiseEpoch(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	epochMilli = (uint64_t) ts.tv_sec * (uint64_t) 1000 + (uint64_t) (ts.tv_nsec / 1000000L);
	epochMicro = (uint64_t) ts.tv_sec * (uint64_t) 1000000 + (uint64_t) (ts.tv_nsec / 1000L);
}

/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay(unsigned int howLong)
{
	struct timespec sleeper, dummy;

	sleeper.tv_sec = (time_t) (howLong / 1000);
	sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;

	nanosleep(&sleeper, &dummy);
}

/*
 * delayMicroseconds:
 *	This is somewhat intersting. It seems that on the Pi, a single call
 *	to nanosleep takes some 80 to 130 microseconds anyway, so while
 *	obeying the standards (may take longer), it's not always what we
 *	want!
 *
 *	So what I'll do now is if the delay is less than 100uS we'll do it
 *	in a hard loop, watching a built-in counter on the ARM chip. This is
 *	somewhat sub-optimal in that it uses 100% CPU, something not an issue
 *	in a microcontroller, but under a multi-tasking, multi-user OS, it's
 *	wastefull, however we've no real choice )-:
 *
 *      Plan B: It seems all might not be well with that plan, so changing it
 *      to use gettimeofday () and poll on that instead...
 *********************************************************************************
 */

void delayMicrosecondsHard(unsigned int howLong)
{
	struct timeval tNow, tLong, tEnd;

	gettimeofday(&tNow, NULL);
	tLong.tv_sec = howLong / 1000000;
	tLong.tv_usec = howLong % 1000000;
	timeradd(&tNow, &tLong, &tEnd);

	while (timercmp(&tNow, &tEnd, <))
		gettimeofday(&tNow, NULL);
}

void delayMicroseconds(unsigned int howLong)
{
	struct timespec sleeper;
	unsigned int uSecs = howLong % 1000000;
	unsigned int wSecs = howLong / 1000000;

	/**/ if (howLong == 0)
		return;
	else if (howLong < 100)
		delayMicrosecondsHard(howLong);
	else {
		sleeper.tv_sec = wSecs;
		sleeper.tv_nsec = (long) (uSecs * 1000L);
		nanosleep(&sleeper, NULL);
	}
}

/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *	Wraps at 49 days.
 *********************************************************************************
 */

unsigned int millis(void)
{
	uint64_t now;

	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	now = (uint64_t) ts.tv_sec * (uint64_t) 1000 + (uint64_t) (ts.tv_nsec / 1000000L);

	return (uint32_t) (now - epochMilli);
}

/*
 * micros:
 *	Return a number of microseconds as an unsigned int.
 *	Wraps after 71 minutes.
 *********************************************************************************
 */

unsigned int micros(void)
{
	uint64_t now;
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	now = (uint64_t) ts.tv_sec * (uint64_t) 1000000 + (uint64_t) (ts.tv_nsec / 1000);

	return (uint32_t) (now - epochMicro);
}

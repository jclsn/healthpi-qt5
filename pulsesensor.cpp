#include "pulsesensor.h"
#include "Heartbeat.h"


#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
     } while (0)

/* VARIABLES FOR GNUPLOT */

const char * plotDataFilename = "./PlotData.dat";
FILE * plotDataFile;
int plotData[DATA_POINTS];

/* VARIABLES USED TO DETERMINE SAMPLE JITTER & TIME OUT */

volatile unsigned int eventCounter, thisTime, lastTime, elapsedTime;
volatile int sumJitter, jitter;
unsigned int timeOutStart, dataRequestStart, m;

/* VARIABLES USED TO DETERMINE BPM */

volatile bool firstBeat = true;     // set these to avoid noise
volatile bool secondBeat = false;    // when we get the heartbeat back
volatile unsigned int sampleCounter;
volatile int pulse_signal;
volatile int threshSetting,lastBeatTime,fadeLevel;
volatile int thresh = 550;      // SHOULD BE ADJUSTABLE ON COMMAND LINE
volatile int peak = 512;           // set P default
volatile int trough = 512;           // set T default
volatile int qs = 0;
volatile int rate[10];
volatile int ibi = 600;         // 600ms per beat = 100 Beats Per Minute (BPM) (inter-beat interval)
volatile bool pulse = false;         // set to 1 while Signal > Threshold
volatile int amp = 100;         // beat amplitude 1/10 of input range.
volatile int bpm = 0;


/* FILE STUFF */
char rawDatafilename [100];
FILE * pulseData;
struct tm *timenow;

/* FUNCTION PROTOTYPES */

void getPulse();
void stopTimer(void);
void initPulseSensorVariables(void);
void initJitterVariables(void);
void initPlotVariables(int idle);
void loadNewPlotData(void);
void incrementPlotData(void);

void sigHandler(int sig_num){
    printf("\nkilling gnuplot and exiting\n");
    system("pkill gnuplot");
    exit(EXIT_SUCCESS);
}

int getBPM()
{
    return bpm;
}

inline int getPulseSensorRaw()
{
    int fd = open("/sys/bus/spi/devices/spi0.0/iio:device0/in_voltage0_raw", O_RDONLY);

    if (fd == -1) {
        fd = open("/sys/bus/spi/devices/spi0.0/iio:device1/in_voltage0_raw", O_RDONLY);
        if (fd == -1) {
            std::cout << "Couldn't open pulse sensor" << std::endl;
            return -1;
        }
    }

    int len, ret;

    if( (len = lseek(fd, 0, SEEK_END)) == (off_t) -1)
        errExit("lseek");

    /* Seek back to the beginning */ 

    if( (lseek(fd, 0, SEEK_SET)) == (off_t) -1)
        errExit("lseek");

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

    if( close(fd) == -1 )
        errExit("close");

    std::string voltage{};

    try {
        voltage = std::string(p2buf);
    }  catch (std::exception e) {
        std::cout << "Could not construct std::string for voltage" << std::endl;
    }

    if(p2buf) {
        free(p2buf);
    }

    return stoi(voltage);
}

int plotBPMData()
{
    while(1)
    {
        delayMicroseconds(1800);    // DELAY TIME NEEDS TO BE ATOMATED
        getPulse();
    }

    return 0;
}

void initPulseSensorVariables(void){

    for (int i = 0; i < 10; ++i)
        rate[i] = 0;

    qs = 0;
    bpm = 0;
    ibi = 420;                  // 420 per beat = 70 Beats Per Minute (BPM)
    pulse = false;
    sampleCounter = 0;
    lastBeatTime = 0;
    peak = 512;                    // Peak at 1/2 the input range of 0..1023
    trough = 512;                    // Trough at 1/2 the input range.
    threshSetting = 550;        // Used to seed and reset the thresh variable
    thresh = 550;               // Threshold a little above the trough
    amp = 100;                  // Beat amplitude 1/10 of input range.
    firstBeat = true;           // Looking for the first beat
    secondBeat = false;         // Not yet looking for the second beat in a row
    lastTime = micros();

}

void getPulse(){     //int sig_num){

    thisTime = micros();								// Time passed since the program has started

    pulse_signal = getPulseSensorRaw();                   	// Get the raw sensor data

    //pulse_signal = -400 + (pulse_signal * pulse_signal) / 600;      // Adjust the sensor graph to fit nicely into GNU plot
    // Also required for the pulse to be detected
    pulse_signal = pulse_signal - 550;

    elapsedTime = thisTime - lastTime;              	// Calculate the time between two measurements
    lastTime = thisTime;                            	// Update the time of the last measurement

    jitter = elapsedTime - 2000;
    sumJitter += jitter;

    sampleCounter += 2;                             	// Keep track of the time in ms with this variable
    int N = sampleCounter - lastBeatTime;           	// Monitor the time since the last beat to avoid noise

    /*  Find the peak and trough of the pulse wave */

    if (pulse_signal < thresh && N > (ibi / 5) * 3) {     	// Avoid dichroitic noise by waiting 3/5 of last IBI (inter-beat interval)
        if (pulse_signal < trough) {
            trough = pulse_signal;                            // Keep track of lowest point in pulse wave
        }
    }

    if (pulse_signal > thresh && pulse_signal > peak) {        		// Thresh condition helps avoid noise
        peak = pulse_signal;
    }                                           		// Keep track of highest point in pulse wave

    /*
     *  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
     *  pulse_signal surges up in value every time there is a pulse
     */

    if (N > 150) {                              		// Avoid high frequency noise

        if ( (pulse_signal > thresh) && (pulse == 0) && (N > ((ibi / 5) * 3)) ) {
            pulse = true;                              	// Set the Pulse flag when we think there is a pulse
            ibi = sampleCounter - lastBeatTime;     	// meaSure time between beats in ms
            lastBeatTime = sampleCounter;           	// keeP track of time for next pulse

            std::cout << "Pound!" << std::endl;
            if (secondBeat) {                       	// if this is the second beat, if secondBeat == TRUE
                enableHeart();
                secondBeat = false;                     // Clear secondBeat flag
                for (int i = 0; i <= 9; i++) {        	// Seed the running total to get a realisitic BPM at startup
                    rate[i] = ibi;
                }
            }

            if (firstBeat) {                       		// if It's the first time we found a beat, if firstBeat == TRUE
                firstBeat = false;                      // Clear firstBeat flag
                secondBeat = true;                      // Set the second beat flag
                // IBI value is unreliable so discard it
                return;
            }

            /* keep a running total of the last 10 IBI values */

            int runningTotal = 0;                  		// Clear the runningTotal variable

            for (int i = 0; i <= 8; i++) {          	// Shift data in the rate array
                rate[i] = rate[i + 1];                	// and drop the oldest IBI value
                runningTotal += rate[i];              	// add up the 9 oldest IBI values
            }

            rate[9] = ibi;                          	// Add the latest IBI to the rate array
            runningTotal += rate[9];                	// Add the latest IBI to runningTotal
            runningTotal /= 10;                     	// Average the last 10 IBI values
            bpm = -5 + 60000 / runningTotal;           	// How many beats can fit into a minute? that's BPM!
            if(bpm < 0)
                bpm = 0;
            qs = 1;                              		// Set Quantified Self flag (we detected a beat)
            //fadeLevel = MAX_FADE_LEVEL;             	// If we're fading, re-light that LED.
            poundHeart();
        }
    }

    if (pulse_signal < thresh && pulse == 1) {  				// When the values are going down, the beat is over
        pulse = false;                         			// Reset the Pulse flag so we can do it again
        amp = peak - trough;                           	// Get amplitude of the pulse wave
        thresh = amp / 2 + trough;                  	// Set thresh at 50% of the amplitude
        peak = thresh;                            		// Reset these for next time
        trough = thresh;
    }

    if (N > 2500) {                          			// If 2.5 seconds go by without a beat
        disableHeart();
        thresh = threshSetting;                			// Set thresh default
        peak = 512;                               		// Set P default
        trough = 512;                             		// Set T default
        lastBeatTime = sampleCounter;          			// Bring the lastBeatTime up to date
        firstBeat = true;                      			// Set these to avoid noise
        secondBeat = false;                    			// When we get the heartbeat back
        qs = 0;
        bpm = 0;
        ibi = 600;                  					// 600ms per beat = 100 Beats Per Minute (BPM)
        pulse = false;
        amp = 100;                  					// Beat amplitude 1/10 of input range.

        std::cout << "Pulsesensor doesn't receive anything!" << std::endl;
    }

#ifdef DEBUG
    std::cout << pulse << "\t" << sampleCounter << "\t" << pulse_signal << "\t " << bpm << "\t " << ibi << "\t " << jitter << std::endl;
#endif
}

void initPlotVariables(int idle)
{
    for(int i=0; i<DATA_POINTS; i++){
        plotData[i] = idle;
    }

    loadNewPlotData();
}

void loadNewPlotData(void)
{
    plotDataFile = fopen(plotDataFilename,"w+");

    for(int i=0; i<DATA_POINTS; i++){
        fprintf(plotDataFile,"%d\t%d\n",i+1,plotData[i]);
    }

    fflush(plotDataFile);
    fclose(plotDataFile);
}

void incrementPlotData(void)
{
    for(int i=0; i<DATA_POINTS-1; i++){
        plotData[i] = plotData[i+1];
    }

    plotData[DATA_POINTS-1] = pulse_signal;
}



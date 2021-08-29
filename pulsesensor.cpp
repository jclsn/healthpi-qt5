#include "pulsesensor.h"

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

volatile int Signal;
volatile unsigned int sampleCounter;
volatile int threshSetting,lastBeatTime,fadeLevel;
volatile int thresh = 550;      // SHOULD BE ADJUSTABLE ON COMMAND LINE
volatile int P = 512;           // set P default
volatile int T = 512;           // set T default
volatile bool firstBeat = true;     // set these to avoid noise
volatile bool secondBeat = false;    // when we get the heartbeat back
volatile int QS = 0;
volatile int rate[10];
int BPM = 0;
volatile int IBI = 600;         // 600ms per beat = 100 Beats Per Minute (BPM)
volatile bool Pulse = false;         // set to 1 while Signal > Threshold
volatile int amp = 100;         // beat amplitude 1/10 of input range.


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
    return BPM;
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

    char * voltage_c = (char *) calloc(7, sizeof(char));

    if( voltage_c == NULL )
        errExit("calloc");

    if( fd == -1 )
        std::cout << "Couldn't access DS1820 voltageerature sensor." << std::endl;

    if( read(fd, voltage_c, 7) == -1 )
        errExit("read");

    if( close(fd) == -1 )
        errExit("close");

    std::string voltage{voltage_c};

    free(voltage_c);

    return stoi(voltage);
}

int plotBPMData()
{
    //signal(SIGINT,sigHandler);

    //const char * rawDatafilename = "./PulseData/PulsePlot.dat";

    //pulseData = fopen(rawDatafilename, "w+");
    //if( pulseData == NULL )
    //    errExit("fopen");

    //fprintf(pulseData,"#Running with delayMicroseconds Timer\n");
    //fprintf(pulseData,"#sampleCount\tSignal\tBPM\tIBI\tjitter\n");

    ///* open gnupolot with a FIFO */

    //FILE * gnuplotPipe = popen("gnuplot -persistent", "w");

    ///* tell gnuplot to read a config file */

    //fprintf(gnuplotPipe,"load 'Pulse_Plot_Config.gnu'\n");

    //initPulseSensorVariables();     // initilaize Pulse Sensor beat finder
    //initPlotVariables(PULSE_SIGNAL_IDLE);  // initialize plot values

    while(1)
    {
        delayMicroseconds(1800);    // DELAY TIME NEEDS TO BE ATOMATED
        getPulse();

        //fprintf(pulseData,"%d\t%d\t%d\t%d\t%d\n",
        //sampleCounter, Signal, BPM, IBI, jitter);

        ///* printf("%d\t%d\t%d\t%d\t%d\n", */
        ///* sampleCounter,Signal,IBI,BPM,jitter */
        ///* ); */

        //if(sampleCounter % 40 == 0){
        //    incrementPlotData();
        //    loadNewPlotData();
        //    fprintf(gnuplotPipe,"plot \"PlotData.dat\" with lines lt rgb \"red\"\n");
        //    fflush(gnuplotPipe);
        //    //int writeTime = micros() - timeOutStart;
        //    //if(writeTime > 2000){
        //        //printf("%d\n",writeTime);
        //    //}
        //}
    }

    return 0;
}

void initPulseSensorVariables(void){

    for (int i = 0; i < 10; ++i)
        rate[i] = 0;

    QS = 0;
    BPM = 0;
    IBI = 600;                  // 600ms per beat = 100 Beats Per Minute (BPM)
    Pulse = false;
    sampleCounter = 0;
    lastBeatTime = 0;
    P = 512;                    // peak at 1/2 the input range of 0..1023
    T = 512;                    // trough at 1/2 the input range.
    threshSetting = 550;        // used to seed and reset the thresh variable
    thresh = 550;               // threshold a little above the trough
    amp = 100;                  // beat amplitude 1/10 of input range.
    firstBeat = true;              // looking for the first beat
    secondBeat = false;             // not yet looking for the second beat in a row
    lastTime = micros();

}


void getPulse(){     //int sig_num){

    thisTime = micros();

    Signal = getPulseSensorRaw();

    /* Adjust graph height */

    Signal = -400 + (Signal * Signal) / 600;

    elapsedTime = thisTime - lastTime;
    lastTime = thisTime;
    jitter = elapsedTime - 2000;
    sumJitter += jitter;

    sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave

    if (Signal < thresh && N > (IBI / 5) * 3) { // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T) {                         // T is the trough
            T = Signal;                             // keep track of lowest point in pulse wave
        }
    }

    if (Signal > thresh && Signal > P) {        // thresh condition helps avoid noise
        P = Signal;                               // P is the peak
    }                                           // keep track of highest point in pulse wave

    /*
     *  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
     *  signal surges up in value every time there is a pulse
     */

    if (N > 150) {                              // avoid high frequency noise

        if ( (Signal > thresh) && (Pulse == 0) && (N > ((IBI / 5) * 3)) ) {
            Pulse = true;                              // set the Pulse flag when we think there is a pulse
            IBI = sampleCounter - lastBeatTime;     // measure time between beats in mS
            lastBeatTime = sampleCounter;           // keep track of time for next pulse

            if (secondBeat) {                       // if this is the second beat, if secondBeat == TRUE
                secondBeat = false;                       // clear secondBeat flag
                for (int i = 0; i <= 9; i++) {        // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;
                }
            }

            if (firstBeat) {                       // if it's the first time we found a beat, if firstBeat == TRUE
                firstBeat = false;                       // clear firstBeat flag
                secondBeat = true;                      // set the second beat flag
                // IBI value is unreliable so discard it
                return;
            }

            /* keep a running total of the last 10 IBI values */

            int runningTotal = 0;                  // clear the runningTotal variable

            for (int i = 0; i <= 8; i++) {          // shift data in the rate array
                rate[i] = rate[i + 1];                // and drop the oldest IBI value
                runningTotal += rate[i];              // add up the 9 oldest IBI values
            }

            rate[9] = IBI;                          // add the latest IBI to the rate array
            runningTotal += rate[9];                // add the latest IBI to runningTotal
            runningTotal /= 10;                     // average the last 10 IBI values
            BPM = 60000 / runningTotal;             // how many beats can fit into a minute? that's BPM!
            QS = 1;                              // set Quantified Self flag (we detected a beat)
            //fadeLevel = MAX_FADE_LEVEL;             // If we're fading, re-light that LED.
        }
    }

    if (Signal < thresh && Pulse == 1) {  // when the values are going down, the beat is over
        Pulse = false;                         // reset the Pulse flag so we can do it again
        amp = P - T;                           // get amplitude of the pulse wave
        thresh = amp / 2 + T;                  // set thresh at 50% of the amplitude
        P = thresh;                            // reset these for next time
        T = thresh;
    }

    if (N > 2500) {                          // if 2.5 seconds go by without a bet
        thresh = threshSetting;                // set thresh default
        P = 512;                               // set P default
        T = 512;                               // set T default
        lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
        firstBeat = true;                      // set these to avoid noise
        secondBeat = false;                    // when we get the heartbeat back
        QS = 0;
        BPM = 0;
        IBI = 600;                  // 600ms per beat = 100 Beats Per Minute (BPM)
        Pulse = false;
        amp = 100;                  // beat amplitude 1/10 of input range.

    }
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

    plotData[DATA_POINTS-1] = Signal;
}



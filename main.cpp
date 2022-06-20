
#include <QQmlApplicationEngine>
#include <QtQuick>
#include <QQmlProperties>

#include "main.h"
#include "debug.h"

unsigned int getButton();
void checkButton();

/* Main Qt function */

int main(int argc, char *argv[])
{


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

       /* Hide the cursor */
    QCursor cursor(Qt::BlankCursor);
    app.setOverrideCursor(cursor);
    app.changeOverrideCursor(cursor);

    QQmlApplicationEngine engine{};

    engine.rootContext()->setContextProperty("tempUpdater", &tempUpdater);
    engine.rootContext()->setContextProperty("gsrUpdater", &gsrUpdater);
    engine.rootContext()->setContextProperty("bpmUpdater", &bpmUpdater);
    engine.rootContext()->setContextProperty("heartbeat", &heartbeat);
    engine.rootContext()->setContextProperty("timelinecntrl", &timelinecntrl);
    engine.rootContext()->setContextProperty("heartfadecntrl", &heartfadecntrl);
    engine.rootContext()->setContextProperty("thermometercntrl", &thermometercntrl);
    engine.rootContext()->setContextProperty("emojicntrl", &emojicntrl);
    engine.rootContext()->setContextProperty("btncntrl", &btncntrl);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect( &engine,
                      &QQmlApplicationEngine::objectCreated,
                      &app,
                      [url](QObject *obj,
                      const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                      },
                      Qt::QueuedConnection);


    engine.load(url);

    std::thread updaterThread(updateValues);

    player = new QMediaPlayer{};
    player->setMedia(QUrl::fromLocalFile("/home/root/sounds/single-heartbeat2.wav"));
    player->setVolume(100);

    return app.exec();
}

/* Function that updates the values in a fixed interval */

void updateValues()
{

    initialiseEpoch();

    /* Create sensor objects */

    DS1820 ds1820{};
    GSRSensor gsrsensor{};

    char pulse_c[20]{};
    std::string pulse{};

    /* Create Pulsesensor object */

    std::thread pulseThread(plotBPMData);

    // std::thread thermoThread(thermometerThread);

    QString temp{},
            gsr{},
            gsr2{},
            gsr3{},
            bpm{};

            float voltage_float{};
    std::string emoji = "images/emoji5a.png";

    timelinecntrl.setEnabled(false);
    btncntrl.setClicked(false);

#ifdef DEBUG_SOUND
    while(1) {
        sleep(1);
        player->play();
    }
#endif

#ifdef DEBUG_HEARTFADE
    heartfadecntrl.setEnabled(true);
    while(1) {
        sleep(1);
        fadeHeart(1);
    }
#endif

    while(1) {

        /*
         *  Wait for start button to be pressed. Since the button is polled via SPI,
         *  the thread cannot must stop while measuring
         */


        if(!timelinecntrl.enabled()) {
            sleeping = true;
            bpmUpdater.setText("0");
            gsrUpdater.setText("0.00 V");
            tempUpdater.setText("0 °C");
            thermometercntrl.setHeight(425);         // Calculate the height of the thermometer and set it
            emojicntrl.setEmoji(QString::fromStdString("images/emoji5a.png"));
            std::thread buttonThread(checkButton);
            buttonThread.join();

            heartfadecntrl.setEnabled(true);
            fadeHeart(5);
            sleep(5);
            sleeping = false;
        }

        /* TEMPERATURE */

        ds1820.readSensor();												// Query the temperature sensor value
        tempUpdater.setText(QString::fromStdString(ds1820.getTempString()));		// Update the text in the GUI
        thermometercntrl.setHeight(ds1820.getThermometerHeight());         // Calculate the height of the thermometer and set it

        /* GALVANIC SKIN RESPONSE SENSOR*/

        gsrsensor.readSensor();
        voltage_float = gsrsensor.getGSRAvgVoltage();
        gsr = QString::fromStdString(gsrsensor.getGSRAvgVoltageString());
        // gsr2 = QString::fromStdString(gsrsensor.getHumanResistance());
        // gsr3 = QString::fromStdString(gsrsensor.getHumanConductance());

        // std::cout << "voltage = " << voltage << ", resistance = " << resistance << ", conductance = " << conductance << std::endl;

        gsrUpdater.setText(gsr);

        if (voltage_float > 4.0)
            emoji = "images/emoji5a.png";

        else if (voltage_float > 3.0 && voltage_float < 4.0 )
            emoji = "images/emoji4.png";

        else if (voltage_float > 2.0 && voltage_float < 3.0 )
            emoji = "images/emoji3.png";

        else if (voltage_float > 1.0 && voltage_float < 2.0 )
            emoji = "images/emoji2.png";

        else if (voltage_float > 0.0 && voltage_float < 1.0 )
            emoji = "images/emoji1a.png";

        emojicntrl.setEmoji(QString::fromStdString(emoji));


        // /* BPM */

        sprintf(pulse_c, "%d", getBPM());
        pulse = std::string(pulse_c);
        bpm = QString::fromStdString(pulse);

        bpmUpdater.setText(bpm);



    }
}

void poundHeart()
{

    float bpm = (float) getBPM();
    if(bpm < 30 || bpm > 200 )
        return;


    qreal rate = (qreal)  (bpm / 60.0) * 1.2;
    player->setPlaybackRate(rate);
    std::cout << "Pound!" << std::endl;
    player->play();

    float animationLength = 1.2 * 500.0 / rate;
    timelinecntrl.setRunning(false);
    timelinecntrl.setEnabled(false);
    timelinecntrl.setFrom(0);
    timelinecntrl.setLoops(1);
    timelinecntrl.setDuration(animationLength);
    timelinecntrl.setStartFrame(0);
    timelinecntrl.setFrame1(1.2 * 50.0/rate);
    timelinecntrl.setFrame2(animationLength-1);
    timelinecntrl.setRunning(true);
    timelinecntrl.setEnabled(true);

}

void fadeHeart(unsigned int times)
{
    // heartfadecntrl.setRunning(false);
    // heartfadecntrl.setLoops(times);
    // heartfadecntrl.setStartFrame(0);
    // heartfadecntrl.setRunning(true);;
    // heartfadecntrl.setEnabled(true);;

    heartfadecntrl.setRunning(false);
    heartfadecntrl.setFrom(0);
    heartfadecntrl.setLoops(times);
    heartfadecntrl.setDuration(1000);
    heartfadecntrl.setStartFrame(0);
    // heartfadecntrl.setFrame1(1.2 * 50.0/rate);
    // heartfadecntrl.setFrame2(animationLength-1);
    heartfadecntrl.setRunning(true);

}

void disableHeart() {
    timelinecntrl.setEnabled(false);
    btncntrl.setClicked(false);
}


void enableHeart()
{
    heartfadecntrl.setEnabled(false);
    timelinecntrl.setEnabled(true);
}

/* Code to read the button which activates the heart rate measurement */

unsigned int getButton()
{
    int fd{};
    char path[60]{};
    int len = 20;
    ssize_t ret{};

    std::string voltage{};

    char *p2buf = (char *) calloc(len, sizeof(char));
    char *buf = p2buf;

    for(int i = 0; i < 10; ++i) {
        sprintf(path, "/sys/bus/spi/devices/spi0.0/iio:device%d/in_voltage1_raw", i);
        fd = open(path, O_RDONLY);
        if(fd > 0)
            break;
    }

    if( fd == -1 ) {
        std::cout << "Couldn't access button interface" << std::endl;
        return -1;
    }


    if( buf == NULL )
        perror("calloc");

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
        perror("close");

    voltage = std::string(p2buf);

    if(p2buf) {
        free(p2buf);
    }

    // std::cout << voltage;

    return stoi(voltage);

}

/* Callback function of the buttonThread */

void checkButton() {

    while(getButton() < 100)
        usleep(10000);

    btncntrl.setClicked(true);
}

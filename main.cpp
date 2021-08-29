#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>
#include <QLocale>
#include <QTranslator>
#include <QQmlProperties>
#include "main.h"


int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "HealthPi_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }


    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("tempUpdater", &tempUpdater);
    engine.rootContext()->setContextProperty("gsrUpdater", &gsrUpdater);
    engine.rootContext()->setContextProperty("bpmUpdater", &bpmUpdater);

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

    return app.exec();
}

void updateValues()
{

#if RASPI

    initialiseEpoch();
    /* Create DS1820 object */

    DS1820 ds1820{};

    /* Create GSRSensor object and the needed strings for conversion */

    GSRSensor gsrsensor{};
    gsrsensor.setupGSR();
    char voltage_c[10];
    char conductance_c[10];
    char resistance_c[10];
    std::string voltage, conductance, resistance;

    char pulse_c[5];
    std::string pulse;

    /* Create Pulsesensor object */

    std::thread pulseThread(plotBPMData);

    for(;;) {


        QString temp = QString::fromStdString(ds1820.getTemp());

        sprintf(voltage_c, "%f", gsrsensor.getGSRVoltage());
        voltage = std::string(voltage_c);
        voltage.replace(3, 7, " V");
        voltage.replace(voltage.find(","), 1, ".");
        QString gsr = QString::fromStdString(voltage);

        sprintf(conductance_c, "%f", (1.0 / gsrsensor.getHumanResistance()) * 1000);
        conductance = std::string(conductance_c);
        QString gsr2 = QString::fromStdString(conductance);

        sprintf(resistance_c, "%d", gsrsensor.getHumanResistance());
        resistance = std::string(resistance_c);
        QString gsr3 = QString::fromStdString(resistance);


        sprintf(pulse_c, "%d", getBPM());
        pulse = std::string(pulse_c);
        QString bpm = QString::fromStdString(pulse);

        tempUpdater.setText(temp);
        gsrUpdater.setText(gsr3);
        bpmUpdater.setText(bpm);
    }

#endif

}

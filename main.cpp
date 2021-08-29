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

    /* Declare DS1820 object */

    DS1820 ds1820{};

    /* Declare GSRSensor object and the needed strings for conversion */

    GSRSensor gsrsensor{};
    gsrsensor.setupGSR();
    char voltage_c[10];
    std::string voltage;

    for(;;) {

        QString temp = QString::fromStdString(ds1820.getTemp());

        sprintf(voltage_c, "%f", gsrsensor.getGSRVoltage());
        voltage = std::string(voltage_c);
        voltage.replace(3, 7, " V");
        voltage.replace(voltage.find(","), 1, ".");
        QString gsr = QString::fromStdString(voltage);

        tempUpdater.setText(temp);
        gsrUpdater.setText(gsr);
    }

#endif

}

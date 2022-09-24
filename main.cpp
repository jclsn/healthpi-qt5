
#include "main.h"

#include "debug.h"

#include <QQmlApplicationEngine>
#include <QQmlProperties>
#include <QtQuick>

unsigned int getButton();
void checkButton();

/* Main Qt function */

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QFontDatabase::addApplicationFont(":/home/root/fonts/IBMPlexMono-Regular.ttf");

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
	engine.rootContext()->setContextProperty("controllight_control", &controllight_control);
	engine.rootContext()->setContextProperty("thermometercntrl", &thermometercntrl);
	engine.rootContext()->setContextProperty("moodcontrol", &moodcontrol);
	engine.rootContext()->setContextProperty("emojicntrl", &emojicntrl);
	engine.rootContext()->setContextProperty("btncntrl", &btncntrl);

	const QUrl url(QStringLiteral("qrc:/main.qml"));

	QObject::connect(
	    &engine,
	    &QQmlApplicationEngine::objectCreated,
	    &app,
	    [url](QObject* obj, const QUrl& objUrl)
	    {
		    if (!obj && url == objUrl)
			    QCoreApplication::exit(-1);
	    },
	    Qt::QueuedConnection);


	engine.load(url);


	player = new QMediaPlayer{};
    player->setMedia(QUrl::fromLocalFile("/home/root/sounds/single-heartbeat2.wav"));
	player->setVolume(100);

    std::thread pulseThread(&Pulsesensor::read_bpm_thread, std::ref(pulsesensor));
    std::thread updaterThread(updateValues);

	return app.exec();
}

/* Function that updates the values in a fixed interval */

void updateValues()
{
	std::string pulse{};

	// std::thread thermoThread(thermometerThread);

    QString temp{}, gsr{}, gsr2{}, gsr3{};

	float voltage_float{};
	std::string emoji = "images/emoji5a.png";

	timelinecntrl.setEnabled(false);
	btncntrl.setClicked(false);

#ifdef DEBUG_SOUND
	while (1) {
		/* sleep(1); */
		std::this_thread::sleep_for(std::chrono::seconds{1});
		player->play();
	}
#endif

#ifdef DEBUG_HEARTFADE
	heartfadecntrl.setEnabled(true);
	while (1) {
		/* sleep(1); */
		std::this_thread::sleep_for(std::chrono::seconds{1});
		fadeHeart(1);
	}
#endif

	while (1) {

		/*
		 *  Wait for start button to be pressed. Since the button is polled via SPI,
         *  the thread must stop while measuring
		 */


		if (!timelinecntrl.enabled()) {

            /* Initial values */

			sleeping = true;
			bpmUpdater.setText("0");
			gsrUpdater.setText("0.00 V");
			tempUpdater.setText("0 °C");
			thermometercntrl.setHeight(650);
			moodcontrol.setHeight(700);
			emojicntrl.setEmoji(QString::fromStdString("images/emoji5a.png"));
			std::thread buttonThread(checkButton);
			buttonThread.join();

			heartfadecntrl.setEnabled(true);
			fadeHeart(5);
			std::this_thread::sleep_for(std::chrono::seconds{5});

			sleeping = false;
		}

		/* TEMPERATURE */

		ds1820.read_sensor();    // Query the temperature sensor value
		tempUpdater.setText(
		    QString::fromStdString(ds1820.get_formatted_temperature()));    // Update the text in the GUI
		thermometercntrl.setHeight(
		    ds1820.get_thermometer_height());    // Calculate the height of the thermometer and set it

		/* GALVANIC SKIN RESPONSE SENSOR*/

		gsrsensor.update_sample_list();
		voltage_float = gsrsensor.get_avg_voltage();
		gsr = QString::fromStdString(gsrsensor.get_avg_voltage_string());
		gsr2 = QString::fromStdString(gsrsensor.get_resistance_string());
		gsr3 = QString::fromStdString(gsrsensor.get_conductance_string());

		// std::cout << "voltage = " << voltage << ", resistance = " << resistance << ", conductance = " << conductance
		// << std::endl;

		gsrUpdater.setText(gsr);

		int bar_height = (700 * voltage_float / 5);
		std::cout << bar_height << "\n";
		moodcontrol.setHeight(bar_height);

		if (voltage_float > 4.0)
			emoji = "images/emoji5a.png";

		else if (voltage_float > 3.0 && voltage_float < 4.0)
			emoji = "images/emoji4.png";

		else if (voltage_float > 2.0 && voltage_float < 3.0)
			emoji = "images/emoji3.png";

		else if (voltage_float > 1.0 && voltage_float < 2.0)
			emoji = "images/emoji2.png";

		else if (voltage_float > 0.0 && voltage_float < 1.0)
			emoji = "images/emoji1a.png";

		emojicntrl.setEmoji(QString::fromStdString(emoji));

		// /* BPM */

        bpmUpdater.setText(QString::fromStdString(std::to_string(pulsesensor.get_bpm())));
	}
}

void poundHeart()
{
    btncntrl.setClicked(true);

	std::cout << "Pound!\n";

    int bpm = pulsesensor.get_bpm();

    std::cout << "BPM = " << pulsesensor.get_bpm() << "\n";

	if (bpm < 30 || bpm > 200)
		return;

	qreal rate = (qreal) (bpm / 60.0) * 1.2;
	player->setPlaybackRate(rate);
	// std::cout << "Pound!" << std::endl;
	player->play();

	float animationLength = 1.2 * 500.0 / rate;
	timelinecntrl.setRunning(false);
	timelinecntrl.setEnabled(false);
	timelinecntrl.setFrom(0);
	timelinecntrl.setLoops(1);
	timelinecntrl.setDuration(animationLength);
	timelinecntrl.setStartFrame(0);
	timelinecntrl.setFrame1(1.2 * 50.0 / rate);
	timelinecntrl.setFrame2(animationLength - 1);
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

void disableHeart()
{
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
	std::filesystem::path button_path{"/sys/bus/spi/devices/spi0.0/iio:device0/in_voltage1_raw"};
	std::ifstream button_handle{button_path};

	/* int fd{}; */
	/* char path[60]{}; */
	/* int len = 20; */
	/* ssize_t ret{}; */

	std::string voltage{};

	/* 	char* p2buf = (char*) calloc(len, sizeof(char)); */
	/* 	char* buf = p2buf; */

	/* 	for (int i = 0; i < 10; ++i) { */
	/* 		sprintf(path, "/sys/bus/spi/devices/spi0.0/iio:device%d/in_voltage1_raw", i); */
	/* 		fd = open(path, O_RDONLY); */
	/* 		if (fd > 0) */
	/* 			break; */
	/* 	} */

	/* 	if (fd == -1) { */
	/* 		std::cout << "Couldn't access button interface" << std::endl; */
	/* 		return -1; */
	/* 	} */


	/* 	if (buf == NULL) */
	/* 		perror("calloc"); */

	/* 	while (len != 0 && (ret = read(fd, buf, len)) != 0) { */
	/* 		if (ret == -1) { */
	/* 			if (errno == EINTR) */
	/* 				continue; */
	/* 			perror("read"); */
	/* 			break; */
	/* 		} */

	/* 		len -= ret; */
	/* 		buf += ret; */
	/* 	} */

	/* 	if (close(fd) == -1) */
	/* 		perror("close"); */

	/* 	voltage = std::string(p2buf); */

	/* 	if (p2buf) */
	/* 		free(p2buf); */

	button_handle >> voltage;
	// std::cout << voltage;

	return stoi(voltage);
}

/* Callback function of the buttonThread */

void checkButton()
{
	while (getButton() < 100) {
		controllight_control.setEnabled(pulsesensor.sensor_is_reading);
		// std::cout << "sensor_is_reading = " << sensor_is_reading << std::endl;
		usleep(1000);
	}

	btncntrl.setClicked(true);
}

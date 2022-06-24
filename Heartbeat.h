#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include <QTimeLine>
#include <qqml.h>

void poundHeart();
void enableHeart();
void disableHeart();

class Heartbeat : public QObject {

	Q_OBJECT
	Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged)
	double m_scale{};

public:
	using QObject::QObject;

	double scale() const
	{
		return m_scale;
	}

public slots:

	void setScale(double scale)
	{
		if (m_scale == scale)
			return;
		m_scale = scale;
		emit scaleChanged(m_scale);
	}

signals:
	void scaleChanged(double scale);
};

#endif    // HEARTBEAT_H

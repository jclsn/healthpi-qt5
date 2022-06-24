#ifndef TIMELINE_CONTROL_H
#define TIMELINE_CONTROL_H

#include <QObject>
#include <QTimeLine>
#include <qqml.h>

class TimelineControl : public QObject {

	Q_OBJECT
	Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
	Q_PROPERTY(unsigned int duration READ duration WRITE setDuration NOTIFY durationChanged)
	Q_PROPERTY(unsigned int startFrame READ startFrame WRITE setStartFrame NOTIFY startFrameChanged)
	Q_PROPERTY(unsigned int frame1 READ frame1 WRITE setFrame1 NOTIFY frame1Changed)
	Q_PROPERTY(unsigned int frame2 READ frame2 WRITE setFrame2 NOTIFY frame2Changed)
	Q_PROPERTY(unsigned int loops READ loops WRITE setLoops NOTIFY loopsChanged)
	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
	Q_PROPERTY(unsigned int from READ from WRITE setFrom NOTIFY fromChanged)

	unsigned int m_running{}, m_duration{}, m_startFrame{}, m_frame1{}, m_frame2{}, m_loops{}, m_from{}, m_enabled{};

public:
	using QObject::QObject;

	bool running() const
	{
		return m_running;
	}

	unsigned int duration() const
	{
		return m_duration;
	}

	unsigned int startFrame() const
	{
		return m_startFrame;
	}

	unsigned int frame1() const
	{
		return m_frame1;
	}

	unsigned int frame2() const
	{
		return m_frame2;
	}

	unsigned int loops() const
	{
		return m_loops;
	}

	bool enabled() const
	{
		return m_enabled;
	}

	bool from() const
	{
		return m_from;
	}

public slots:

	void setRunning(bool running)
	{
		if (m_running == running)
			return;
		m_running = running;
		emit runningChanged(m_running);
	}

	void setDuration(unsigned int duration)
	{
		if (m_duration == duration)
			return;
		m_duration = duration;
		emit durationChanged(m_duration);
	}

	void setStartFrame(unsigned int startFrame)
	{
		if (m_startFrame == startFrame)
			return;
		m_startFrame = startFrame;
		emit startFrameChanged(m_startFrame);
	}

	void setFrame1(unsigned int frame1)
	{
		if (m_frame1 == frame1)
			return;
		m_frame1 = frame1;
		emit frame1Changed(m_frame1);
	}

	void setFrame2(unsigned int frame2)
	{
		if (m_frame2 == frame2)
			return;
		m_frame2 = frame2;
		emit frame2Changed(m_frame2);
	}

	void setLoops(unsigned int loops)
	{
		if (m_loops == loops)
			return;
		m_loops = loops;
		emit loopsChanged(m_loops);
	}

	void setEnabled(bool enabled)
	{
		if (m_enabled == enabled)
			return;
		m_enabled = enabled;
		emit enabledChanged(m_enabled);
	}

	void setFrom(unsigned int from)
	{
		if (m_from == from)
			return;
		m_from = from;
		emit fromChanged(m_from);
	}

signals:
	void runningChanged(bool running);
	void durationChanged(unsigned int duration);
	void startFrameChanged(unsigned int startFrame);
	void frame1Changed(unsigned int frame1);
	void frame2Changed(unsigned int frame2);
	void loopsChanged(int loops);
	void enabledChanged(bool enabled);
	void fromChanged(unsigned int from);
};

#endif    // TIMELINE_CONTROL_H

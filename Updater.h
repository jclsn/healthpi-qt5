#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater : public QObject {

	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
	QString m_text{};

public:
	using QObject::QObject;

	QString text() const
	{
		return m_text;
	}

public slots:

	void setText(QString text)
	{
		if (m_text == text)
			return;
		m_text = text;
		emit textChanged(m_text);
	}

signals:
	void textChanged(QString text);
};

#endif    // UPDATER_H

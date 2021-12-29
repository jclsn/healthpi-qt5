#ifndef THERMOMETERCONTROL_H
#define THERMOMETERCONTROL_H

#include <QObject>
#include <QTimeLine>
#include <qqml.h>

class ThermometerControl: public QObject{

    Q_OBJECT
    Q_PROPERTY(unsigned int height READ height WRITE setHeight NOTIFY heightChanged)

    unsigned int m_height;

public:
    using QObject::QObject;
    unsigned int height() const{
        return m_height;
    }

public slots:
    void setHeight(unsigned int height){
        if (m_height == height)
            return;
        m_height = height;
        emit heightChanged(m_height);
    }

signals:
    void heightChanged(unsigned int height);
};

#endif // THERMOMETERCONTROL_H

#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include <QObject>
#include <QTimeLine>
#include <qqml.h>

class ButtonControl: public QObject{

    Q_OBJECT
    Q_PROPERTY(bool clicked READ clicked WRITE setClicked NOTIFY clickedChanged)
    Q_PROPERTY(QString statusText READ statusText WRITE setStatusText NOTIFY statusTextChanged)

    bool m_clicked;
    QString m_statusText = "deactivated";

public:
    using QObject::QObject;
    bool clicked() const{
        return m_clicked;
    }
    QString statusText() const{
        return m_statusText;
    }

public slots:
    void setStatusText(QString statusText){
        if (m_statusText == statusText)
            return;
        m_statusText = statusText;
        emit statusTextChanged(m_statusText);
    }

    void setClicked(bool clicked){
        if (m_clicked == clicked)
            return;
        m_clicked = clicked;
        if(m_clicked)
            setStatusText("activated");
        else
            setStatusText("deactivated");
        emit clickedChanged(m_clicked);
    }

signals:
    void clickedChanged(bool clicked);
    void statusTextChanged(QString statusText);
};

#endif // BUTTON_CONTROL_H

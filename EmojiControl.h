#ifndef EMOJICONTROL_H
#define EMOJICONTROL_H

#include <QObject>
#include <QTimeLine>
#include <qqml.h>
#include <string>

class EmojiControl: public QObject{

    Q_OBJECT
    Q_PROPERTY(QString emoji READ emoji WRITE setEmoji NOTIFY emojiChanged)

    QString m_emoji;

public:
    using QObject::QObject;
    QString emoji() const{
        return m_emoji;
    }

public slots:
    void setEmoji(QString emoji){

        if (m_emoji == emoji)
            return;
        m_emoji = emoji;
        emit emojiChanged(m_emoji);
    }

signals:
    void emojiChanged(QString emoji);
};

#endif // EMOJICONTROL_H

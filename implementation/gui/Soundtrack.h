#ifndef SOUNDTRACK_H
#define SOUNDTRACK_H

#include <QObject>

class QMediaPlayer;

namespace Gui
{
    class Soundtrack
        : public QObject
    {
        Q_OBJECT

    public:

        ~Soundtrack();

        static Soundtrack & instance();

        void increaseVolume();

        void decreaseVolume();

    private:

        QMediaPlayer * mPlayer;

        Soundtrack();

        Soundtrack(const Soundtrack & other);

        Soundtrack & operator =(const Soundtrack & other);

        static Soundtrack * _instance;

    };
}

#endif

#ifndef NEWGAMEPANEL_H
#define NEWGAMEPANEL_H

#include <QFrame>

class QBoxLayout;

namespace Gui
{
    class NewGamePanel
        : public QFrame
    {
        Q_OBJECT

    public:

        ~NewGamePanel();

        NewGamePanel(QWidget * parent);

    private slots:

        void slot_editingFinished();

    private:

        QBoxLayout * mPlayerNames;

        NewGamePanel();

        NewGamePanel(const NewGamePanel & other);

        NewGamePanel & operator =(const NewGamePanel & other);

    };
}

#endif
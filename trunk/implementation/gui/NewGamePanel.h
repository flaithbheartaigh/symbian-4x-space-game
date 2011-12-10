#ifndef NEWGAMEPANEL_H
#define NEWGAMEPANEL_H

#include <QFrame>
#include <vector>
#include <string>

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

        void slot_valueChanged(int);

    private:

        QBoxLayout * mPlayerNames;

        std::vector<std::string> mPlayerNamesStr;

        NewGamePanel();

        NewGamePanel(const NewGamePanel & other);

        NewGamePanel & operator =(const NewGamePanel & other);

    };
}

#endif
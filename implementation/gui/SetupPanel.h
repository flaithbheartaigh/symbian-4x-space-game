#ifndef SETUPPANEL_H
#define SETUPPANEL_H

#include <QFrame>

namespace Gui
{
    class SetupPanel
        : public QFrame
    {
        Q_OBJECT

    public:

        ~SetupPanel();

        SetupPanel(QWidget * parent);

    private:

        SetupPanel();

        SetupPanel(const SetupPanel & other);

        SetupPanel & operator =(const SetupPanel & other);

    };
}

#endif
#ifndef HELPPANEL_H
#define HELPPANEL_H

#include <QFrame>

namespace Gui
{
    class HelpPanel
        : public QFrame
    {
        Q_OBJECT

    public:

        ~HelpPanel();

        HelpPanel(QWidget * parent);

    private:

        HelpPanel();

        HelpPanel(const HelpPanel & other);

        HelpPanel & operator =(const HelpPanel & other);

    };
}

#endif
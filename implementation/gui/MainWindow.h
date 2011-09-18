#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <set>

class QBoxLayout;
class QResizeEvent;
class QKeyEvent;
class QStackedWidget;

namespace Gui
{
    class MainWindow
        : public QFrame
    {
        Q_OBJECT

    public:

        static const int MainFrameIndex;

        static const int StatsFrameIndex;

        static const int ShipDesignIndex;

        static const int ShipBuildIndex;

        static const int ComponentsIndex;

        static const int SetupIndex;

        static const int HelpIndex;

        static MainWindow & instance();

        ~MainWindow();

        void showFrame(int index);

        QWidget * getFrame(int index) const;

    protected:

        void resizeEvent(QResizeEvent * event);

    private:

        QStackedWidget * mStack;

        static MainWindow * _instance;

        MainWindow();

        MainWindow(const MainWindow & other);
        
        MainWindow & operator =(const MainWindow & other);
        
    };
}

#endif

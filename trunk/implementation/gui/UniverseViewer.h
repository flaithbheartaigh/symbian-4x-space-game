#ifndef UNIVERSEVIEWER_H
#define UNIVERSEVIEWER_H

#include <QWidget>

#include <set>

class QGraphicsScene;
class QGraphicsItem;
class QResizeEvent;
class QSlider;
class QItemSelectionModel;
class QListView;

namespace Game
{
    class Sector;
    class UniverseVisitor;
}

namespace Gui
{
    class UniverseViewer
        : public QWidget
    {

    Q_OBJECT

    public:

        ~UniverseViewer();

        UniverseViewer(QWidget * parent);

        void centerOn(Game::Sector * sector);

        QItemSelectionModel * sectorSelectionModel() const;

        void noForce(bool noForce);

    protected:

        void resizeEvent(QResizeEvent * event);

    private slots:

        void slot_valueChanged(int value);

    private:

        QGraphicsScene * mGraphicsScene;

        QSlider * mZoomSlider;

        QListView * mSectorView;

        UniverseViewer(const UniverseViewer & other);
        
        UniverseViewer & operator =(const UniverseViewer & other);
        
    };
}

#endif
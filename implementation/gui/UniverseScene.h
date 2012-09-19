#ifndef UNIVERSESCENE_H
#define UNIVERSESCENE_H

#include <QGraphicsScene>

#include <game/Universe.h>

#include <map>

class QGraphicsSceneMouseEvent;

namespace Gui
{
    class UniverseViewer;
    class StarSystemGraphicsItem;

    class UniverseScene
        : public QGraphicsScene
        , private Game::Universe::Subscriber
    {

    public:

        ~UniverseScene();

        UniverseScene(UniverseViewer * universeViewer);

    protected:

        void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

    private:

        void starSystemAdded(Game::StarSystem * starSystem);

        void starSystemRemoved(Game::StarSystem * starSystem);

        std::map<Game::StarSystem *, StarSystemGraphicsItem *> mItems;

        int mW;

        int mH;

        UniverseScene(const UniverseScene & other);
        
        UniverseScene & operator =(const UniverseScene & other);
        
    };
}

#endif
#ifndef STARSYSTEMGRAPHICSITEM_H
#define STARSYSTEMGRAPHICSITEM_H

#include <QGraphicsItem>

#include <game/StarSystem.h>
#include <game/Universe.h>

#include <map>

class QGraphicsTextItem;

namespace Gui
{
    class SectorGraphicsItem;

    class StarSystemGraphicsItem
        : public QGraphicsItem
        , private Game::StarSystem::Subscriber
        , private Game::Universe::Game::Subscriber
    {

    public:

        static QPointF scenePosition(Game::StarSystem * starSystem);

        ~StarSystemGraphicsItem();

        StarSystemGraphicsItem(QGraphicsItem * parent, Game::StarSystem * starSystem);

        QRectF boundingRect() const;

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        void sectorAdded(Game::Sector * sector);

        void sectorRemoved(Game::Sector * sector);

        void becameKnown();

        void playerActivated(Game::Player * player);

        std::map<Game::Sector *, SectorGraphicsItem *> mItems;

        QGraphicsSimpleTextItem * mText;

        StarSystemGraphicsItem(const StarSystemGraphicsItem & other);
        
        StarSystemGraphicsItem & operator =(const StarSystemGraphicsItem & other);

    };
}

#endif
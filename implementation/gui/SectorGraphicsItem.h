#ifndef SECTORGRAPHICSITEM_H
#define SECTORGRAPHICSITEM_H

#include <QGraphicsItem>

#include <game/Sector.h>

class QGraphicsLineItem;

namespace Gui
{
    class SectorGraphicsItem
        : public QGraphicsItem
        , public Game::Sector::Subscriber
    {

    public:

        static QPointF scenePositionAbsolute(Game::Sector * sector);

        static QPointF scenePositionRelative(Game::Sector * sector);

        ~SectorGraphicsItem();

        SectorGraphicsItem(QGraphicsItem * parent, Game::Sector * sector);

        QRectF boundingRect() const;

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        void contentsChanged(Game::Sector::Content changed, bool forcedRedraw);

        void selected();

        void deselected();

        bool mIsSelected;

        QGraphicsLineItem * mLine;

        SectorGraphicsItem(const SectorGraphicsItem & other);
        
        SectorGraphicsItem & operator =(const SectorGraphicsItem & other);
        
    };
}

#endif
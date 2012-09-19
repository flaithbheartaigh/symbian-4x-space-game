#ifndef SECTORGRAPHICSITEM_H
#define SECTORGRAPHICSITEM_H

#include <QGraphicsItem>

#include <game/Sector.h>

namespace Gui
{
    class SectorGraphicsItem
        : public QGraphicsItem
        , public Game::Sector::Subscriber
    {

    public:

        ~SectorGraphicsItem();

        SectorGraphicsItem(QGraphicsItem * parent, Game::Sector * sector);

        QRectF boundingRect() const;

        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    private:

        void contentsChanged(Game::Sector::Content changed, bool forcedRedraw);

        void selected();

        void deselected();

        bool mIsSelected;

        SectorGraphicsItem(const SectorGraphicsItem & other);
        
        SectorGraphicsItem & operator =(const SectorGraphicsItem & other);
        
    };
}

#endif
// Symbian 4X Space Game
// 
// Copyright (C) 2011 Patrick Pelletier
// 
// This program is free software: you can redistribute it and/or modify it
// under the  terms of the GNU  General Public License as published by the
// Free Software Foundation, either version 3 of the License, or any later
// version.
// 
// This  program  is distributed  in  the  hope  that  it will  be  useful,
// but  WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// 
// You should have received a copy  of the GNU General Public License along
// with this program. See <http://www.opensource.org/licenses/gpl-3.0.html>

#include "StarSystemGraphicsItem.h"
#include "SectorGraphicsItem.h"
#include "MainWindow.h"
#include "UniverseViewer.h"

#include <game/Player.h>
#include <game/StarSystem.h>
#include <game/Sector.h>

#include <QBrush>
#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

#define USE_CLIPPING 0

using namespace Gui;

QPointF StarSystemGraphicsItem::scenePosition(Game::StarSystem * starSystem)
{
    if (starSystem == NULL)
    {
        return QPointF();
    }
    return QPointF(starSystem->x()*Game::StarSystem::Size*UniverseViewer::ReferenceSize, starSystem->y()*Game::StarSystem::Size*UniverseViewer::ReferenceSize);
}

StarSystemGraphicsItem::~StarSystemGraphicsItem()
{
    delete mText;
    mText = NULL;
}

StarSystemGraphicsItem::StarSystemGraphicsItem(QGraphicsItem * parent, Game::StarSystem * starSystem)
    : QGraphicsItem(parent)
    , Game::StarSystem::Subscriber(starSystem)
    , Game::Universe::Game::Subscriber()
    , mItems()
    , mText(NULL)
{
    if (Gui::MainWindow::Settings_CacheMode >= 3)
    {
        setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
    }
}

QRectF StarSystemGraphicsItem::boundingRect() const
{
    return QRectF(
        -Game::StarSystem::Size/2.0*UniverseViewer::ReferenceSize,
        -Game::StarSystem::Size/2.0*UniverseViewer::ReferenceSize,
         Game::StarSystem::Size    *UniverseViewer::ReferenceSize, 
         Game::StarSystem::Size    *UniverseViewer::ReferenceSize);
}

void StarSystemGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if (starSystem() != NULL)
    {
#if USE_CLIPPING
        if (Gui::MainWindow::Settings_CacheMode < 2)//3
        {
            painter->setClipRect(option->exposedRect);
        }
        else
        {
            //painter->setClipRect(boundingRect());
        }
#endif
        if (Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(starSystem()))
        {
            std::set<Game::Player *> players = starSystem()->players();
            for (std::set<Game::Player *>::iterator it = players.begin(); it != players.end(); ++it)
            {
                QPen previousPen = painter->pen();
                std::vector<int> colorComponents = Game::Player::color(Game::Universe::instance().game().playerIndex(*it));
                QColor color(colorComponents[0], colorComponents[1], colorComponents[2]);
                if (Gui::MainWindow::Settings_DetailLevel > 1)
                {
                    color.setAlphaF(0.1f);
                    painter->fillRect(boundingRect(), QBrush(color));
                }
                color.setAlphaF(0.35f);
                painter->setPen(QPen(color, 9));
                painter->drawRect(boundingRect());
                painter->setPen(previousPen);
            }
        }
    }
}

void StarSystemGraphicsItem::sectorAdded(Game::Sector * sector)
{
    if (sector != NULL)
    {
        SectorGraphicsItem * sectorGraphicsItem = new SectorGraphicsItem(this, sector);
        sectorGraphicsItem->setPos(SectorGraphicsItem::scenePositionRelative(sector));
        mItems[sector] = sectorGraphicsItem;
    }
    if (mText == NULL)
    {
        class TextItem
            : public QGraphicsSimpleTextItem
        {
        public:
            TextItem(const QString & text)
                : QGraphicsSimpleTextItem(text)
            {

            }
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
            {
                painter->translate(boundingRect().topLeft());
                QGraphicsSimpleTextItem::paint(painter, option, widget);
                painter->translate(-boundingRect().topLeft());
            }
            QRectF boundingRect() const
            {
                QRectF b = QGraphicsSimpleTextItem::boundingRect();
                return QRectF(b.x()-b.width()/2.0, b.y()-b.height()/2.0, b.width(), b.height());
            }
        };
        mText = new TextItem(QString::fromStdString(starSystem()->name()));
        scene()->addItem(mText);
        mText->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        mText->setFont(QFont("tahoma", 10));
        mText->setPos(pos() + QPointF(0.0, UniverseViewer::ReferenceSize/2.0));
        mText->setBrush(QBrush(Qt::white));
        mText->setVisible(Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(starSystem()));
    }
}

void StarSystemGraphicsItem::sectorRemoved(Game::Sector * sector)
{
    if (sector != NULL && mItems[sector] != NULL)
    {
        mItems[sector]->setParentItem(NULL);
        delete mItems[sector];
        mItems.erase(sector);
    }
}

void StarSystemGraphicsItem::becameKnown()
{
    if (Gui::MainWindow::Settings_CacheMode >= 3)
    {
        QList<QGraphicsItem *> children = childItems();
        for (int i = 0; i < children.size(); ++i)
        {
            children[i]->update();
        }
    }
    if (mText != NULL)
    {
        mText->show();
    }
}

void StarSystemGraphicsItem::playerActivated(Game::Player * player)
{
    if (mText != NULL)
    {
        mText->setVisible(Gui::MainWindow::Settings_ViewUniverse || player == NULL || player->knows(starSystem()));
    }
}
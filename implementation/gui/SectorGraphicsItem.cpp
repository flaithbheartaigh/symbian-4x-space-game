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

#include "SectorGraphicsItem.h"
#include "StarSystemGraphicsItem.h"
#include "MainWindow.h"
#include "UniversePainter.h"
#include "UniverseViewer.h"

#include <game/Universe.h>
#include <game/Player.h>
#include <game/StarSystem.h>
#include <game/Warp.h>

#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>

#define USE_CLIPPING 0

using namespace Gui;

QPointF SectorGraphicsItem::scenePositionAbsolute(Game::Sector * sector)
{
    if (sector == NULL)
    {
        return QPointF();
    }
    return StarSystemGraphicsItem::scenePosition(sector->starSystem()) + SectorGraphicsItem::scenePositionRelative(sector);
}

QPointF SectorGraphicsItem::scenePositionRelative(Game::Sector * sector)
{
    if (sector == NULL)
    {
        return QPointF();
    }
    return QPointF(sector->x()*UniverseViewer::ReferenceSize, sector->y()*UniverseViewer::ReferenceSize);
}

SectorGraphicsItem::~SectorGraphicsItem()
{
    delete mLine;
    mLine = NULL;
}

SectorGraphicsItem::SectorGraphicsItem(QGraphicsItem * parent, Game::Sector * sector)
    : QGraphicsItem(parent)
    , Game::Sector::Subscriber(sector)
    , Game::Universe::Game::Subscriber()
    , mIsSelected(false)
    , mLine(NULL)
{
    if (Gui::MainWindow::Settings_SkipEmptyTiles)
    {
        setFlag(QGraphicsItem::ItemHasNoContents, sector->isEmpty() && !mIsSelected);
    }
    if (Gui::MainWindow::Settings_CacheMode >= 1)
    {
        setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(boundingRect().size().width(),boundingRect().size().height()));
    }
    if (sector->contents() != Game::Sector::NoContents)
    {
        contentsChanged(sector->contents());
    }
}

void SectorGraphicsItem::contentsChanged(Game::Sector::Content changed, bool forcedRedraw)
{
    update();
    if (Gui::MainWindow::Settings_SkipEmptyTiles)
    {
        if (flags() & QGraphicsItem::ItemHasNoContents)
        {
            if (!scene()->views()[0]->property("NoForce").toBool() || Game::Universe::instance().game().currentPlayer()->isHuman())
            {
                setFlag(QGraphicsItem::ItemHasNoContents, false);
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
            }
            setFlag(QGraphicsItem::ItemHasNoContents, sector()->isEmpty() && !mIsSelected);
        }
    }
    else if (forcedRedraw)
    {
        if (!scene()->views()[0]->property("NoForce").toBool() || Game::Universe::instance().game().currentPlayer()->isHuman())
        {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
        }
    }
    
    if (changed & Game::Sector::HasWarp)
    {
        if (mLine == NULL && sector()->warp()->destination().isValid())
        {
            QPointF from = scenePositionAbsolute(sector());
            QPointF to = scenePositionAbsolute(sector()->warp()->destination().sector());
            to = to - ((to - from) / 2);
            mLine = scene()->addLine(from.x(), from.y(), to.x(), to.y(), QPen(QColor(63,127,195,63), 24));
            mLine->setZValue(-1);
            mLine->setVisible(Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(sector()->starSystem()));
        }
    }
}

void SectorGraphicsItem::selected()
{
    mIsSelected = true;

    if (Gui::MainWindow::Settings_SkipEmptyTiles)
    {
        setFlag(QGraphicsItem::ItemHasNoContents, false);
    }

    update();
}

void SectorGraphicsItem::deselected()
{
    mIsSelected = false;

    update();

    if (Gui::MainWindow::Settings_SkipEmptyTiles)
    {
        if (!scene()->views()[0]->property("NoForce").toBool())
        {
            setFlag(QGraphicsItem::ItemHasNoContents, false);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
        }
        setFlag(QGraphicsItem::ItemHasNoContents, sector()->isEmpty() && !mIsSelected);
    }
}

QRectF SectorGraphicsItem::boundingRect() const
{
    return QRectF(
        -0.5*UniverseViewer::ReferenceSize,
        -0.5*UniverseViewer::ReferenceSize,
           1*UniverseViewer::ReferenceSize,
           1*UniverseViewer::ReferenceSize);
}

void SectorGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
#if USE_CLIPPING
    if (Gui::MainWindow::Settings_CacheMode < 1)
    {
        painter->setClipRect(option->exposedRect);
    }
    else
    {
        painter->setClipRect(boundingRect());
    }
#endif
    Gui::UniversePainter().paintSector(painter, sector(), boundingRect().size(), mIsSelected, Gui::MainWindow::Settings_DetailLevel, 
        Gui::MainWindow::Settings_ViewUniverse || Game::Universe::instance().game().currentPlayer() == NULL || Game::Universe::instance().game().currentPlayer()->knows(sector()->starSystem()));
}

void SectorGraphicsItem::becameKnown()
{
    if (mLine != NULL)
    {
        mLine->show();
    }
}

void SectorGraphicsItem::playerActivated(Game::Player * player)
{
    if (mLine != NULL)
    {
        mLine->setVisible(Gui::MainWindow::Settings_ViewUniverse || player == NULL || player->knows(sector()->starSystem()));
    }
}
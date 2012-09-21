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

#include "UniverseScene.h"
#include "StarSystemGraphicsItem.h"
#include "SectorGraphicsItem.h"
#include "UniverseViewer.h"

#include <game/Player.h>
#include <game/StarSystem.h>

#include <QGraphicsSceneMouseEvent>

using namespace Gui;

UniverseScene::~UniverseScene()
{

}

UniverseScene::UniverseScene(UniverseViewer * universeViewer)
    : QGraphicsScene(universeViewer)
    , Game::Universe::Subscriber()
    , mItems()
    , mW(0)
    , mH(0)
{
    mW = QImage(QSize(16,16), QImage::Format_Mono).physicalDpiX() / 4;
    mH = QImage(QSize(16,16), QImage::Format_Mono).physicalDpiY() / 4;
    if (mW < 4 || mW > 1000 || mH < 4 || mH > 1000)
    {
        mW = 32;
        mH = 32;
    }
}

void UniverseScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if ((mouseEvent->buttonDownScenePos(Qt::LeftButton) - mouseEvent->lastScenePos()).manhattanLength() < mW+mH)
    {
        QList<QGraphicsItem *> selectedItems = items(mouseEvent->lastScenePos());
        if (!selectedItems.empty())
        {
            SectorGraphicsItem * sectorItem = dynamic_cast<SectorGraphicsItem *>(selectedItems.first());
            if (sectorItem != NULL && Game::Universe::instance().game().currentPlayer() != NULL)
            {
                if (sectorItem->sector() != Game::Universe::instance().game().currentPlayer()->selectedSector().sector())
                {
                    Game::Universe::instance().game().currentPlayer()->setSelectedSector(sectorItem->sector());
                }
                else
                {
                    Game::Universe::instance().game().currentPlayer()->reselectSector();
                }
            }
        }
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void UniverseScene::starSystemAdded(Game::StarSystem * starSystem)
{
    if (starSystem != NULL)
    {
        StarSystemGraphicsItem * starSystemGraphicsItem = new StarSystemGraphicsItem(NULL, starSystem);
        starSystemGraphicsItem->setPos(StarSystemGraphicsItem::scenePosition(starSystem));
        addItem(starSystemGraphicsItem);
        mItems[starSystem] = starSystemGraphicsItem;
    }
}

void UniverseScene::starSystemRemoved(Game::StarSystem * starSystem)
{
    if (starSystem != NULL)
    {
        removeItem(mItems[starSystem]);
        delete mItems[starSystem];
        mItems.erase(starSystem);
    }
}
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

#include "DeserializeVisitor.h"

#include "Technology.h"
#include "Universe.h"
#include "Player.h"
#include "StarSystem.h"
#include "Sector.h"
#include "Star.h"
#include "Planet.h"
#include "Ship.h"
#include "ShipYard.h"
#include "SectorReference.h"

using namespace Game;

DeserializeVisitor::~DeserializeVisitor()
{
    
}

DeserializeVisitor::DeserializeVisitor()
    : UniverseVisitor()
    , mSector(NULL)
{

}

void DeserializeVisitor::visit(Universe * universe)
{
    if (universe != NULL)
    {
        //@todo review 1
        std::set<Universe::Subscriber *> subscribers = universe->mSubscribers;
        for (std::set<Universe::Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->notify();
        }
        universe->game().setCurrentPlayerIndex(universe->game().currentPlayerIndex());
    }
}

void DeserializeVisitor::visit(StarSystem * starSystem)
{
    if (starSystem != NULL)
    {
        for (int y = -StarSystem::Size / 2; y <= StarSystem::Size / 2; ++y)
        {
            for (int x = -StarSystem::Size / 2; x <= StarSystem::Size / 2; ++x)
            {
                if (!SectorReference(starSystem, x, y).isValid())
                {
                    Sector * sector = new Sector(starSystem);
                    sector->setX(x);
                    sector->setY(y);
                    starSystem->addSector(sector);
                }
            }
        }
        //@todo review 1
        std::set<StarSystem::Subscriber *> subscribers = starSystem->mSubscribers;
        for (std::set<StarSystem::Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->notify();
        }
    }
}

void DeserializeVisitor::visit(Sector * sector)
{
    mSector = sector;
}

void DeserializeVisitor::visit(Star * star)
{

}

void DeserializeVisitor::visit(Planet * planet)
{
    planet->setSector(mSector);
    planet->setPlayerName(planet->playerName());
}

void DeserializeVisitor::visit(Ship * ship)
{
    ship->setSector(mSector);
    ship->setPlayerName(ship->playerName());
}

void DeserializeVisitor::visit(Shipyard * shipyard)
{

}

void DeserializeVisitor::visit(Player * player)
{
    if (Universe::instance().game().currentPlayer() == player)
    {
        Universe::instance().game().setCurrentPlayerIndex(Universe::instance().game().currentPlayerIndex());
    }
}
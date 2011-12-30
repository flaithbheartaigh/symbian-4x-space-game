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

#include "ShipMovement.h"
#include "StarSystem.h"
#include "SectorReference.h"
#include "Sector.h"
#include "Ship.h"
#include "Player.h"
#include "Technology.h"

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h> //Sleep
#else
    #include <unistd.h>  //sleep
#endif

namespace
{
    void sleep500()
    {
#ifdef _WIN32
        Sleep(250);
#else
        usleep(250000);
#endif
    }
}

using namespace Game;

ShipMovement::~ShipMovement()
{

}

ShipMovement::ShipMovement()
    : mShips()
{

}

void ShipMovement::addShip(Ship * ship)
{
    mShips.push_back(ship);
}

void ShipMovement::addShips(const std::vector<Ship *> & ships)
{
    for (std::vector<Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
    {
        addShip(*it);
    }
}

void ShipMovement::setDestination(Sector * sector)
{
    for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
    {
        if ((*it)->canMoveTo(sector))
        {
            (*it)->setDestination(sector);
            if (sector->starSystem() != (*it)->sector()->starSystem())
            {
                int highestID = (*it)->config().highestID(Game::Component::StarDrive);
                if (highestID >= 0 && highestID < static_cast<int>(Game::Technology::instance().starDriveModules().size()))
                {
                    float distance = (*it)->sector()->starSystem()->distance(sector->starSystem());
                    (*it)->setArrival(Game::Technology::instance().starDriveModules()[highestID].arrival(distance));
                    (*it)->setMovement(0);
                }
            }
        }
    }
}

void ShipMovement::run()
{
    std::map<Sector *, std::vector<Ship *> > movementMap;
    do
    {
        if (!movementMap.empty() && !movementMap.begin()->second.empty() && movementMap.begin()->second.front()->player()->isHuman())
        {
            sleep500();
        }
        movementMap.clear();
        for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
        {
            Ship * ship = *it;
            if (ship->movement() >= 1 && ship->canMoveTo(ship->destination().sector()))
            {
                Game::Sector * sector = ship->sector()->nextSectorInPath(ship->destination().sector());
                if (sector != NULL)
                {
                    movementMap[ship->sector()->nextSectorInPath(ship->destination().sector())].push_back(ship);
                    ship->sector()->removeShip(ship);
                    if (ship->movement() >= 1)
                    {
                        ship->setMovement(ship->movement() - 1);
                    }
                }
            }
        }
        std::vector<Ship *> movedShips;
        for (std::map<Sector *, std::vector<Ship *> >::const_iterator it = movementMap.begin(); it != movementMap.end(); ++it)
        {
            (*it).first->addShips((*it).second);
        }
    }
    while (!movementMap.empty());

    mShips.clear();
}
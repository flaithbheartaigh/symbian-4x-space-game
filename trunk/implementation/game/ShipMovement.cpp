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
/*
    void warp(Game::Ship * ship, Game::Sector * destination)
    {
        int highestID = ship->config().highestID(Game::Component::StarDrive);
        if (highestID >= 0 && highestID < static_cast<int>(Game::Technology::instance().starDriveModules().size()))
        {
            float distance = ship->sector()->starSystem()->distance(destination->starSystem());
            ship->setArrival(Game::Technology::instance().starDriveModules()[highestID].arrival(distance));
            ship->setDelayTurns(Game::Technology::instance().starDriveModules()[highestID].delayTurns());
        }
        ship->setMovement(0);
    }

    void move(Game::Ship * ship, Game::Sector * destination)
    {
        Game::Sector * next = ship->sector()->nextSectorInPath(destination);
        if (next != NULL)
        {
            //the following two lines modify variable mSector
            ship->sector()->removeShip(ship);
            if (ship->movement() >= 1)
            {
                ship->setMovement(ship->movement() - 1);
            }
            next->addShip(ship, true); 
            if (next == destination)
            {
                ship->setDestination(Game::SectorReference());
            }
        }
    }
*/
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
{/*
    if (mCurrent->starSystem()  mDestination->starSystem())
    {
        for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
        {*/
            /*
            Ship * ship = *it;
            ship->setDelayTurns(0);
            ship->setArrival(0);
            ship->setDestination(SectorReference(mDestination));
            */
            /*
            int highestID = ship->config().highestID(Game::Component::StarDrive);
            if (highestID >= 0 && highestID < static_cast<int>(Game::Technology::instance().starDriveModules().size()))
            {
                float distance = ship->sector()->starSystem()->distance(mDestination->starSystem());
                ship->setArrival(Game::Technology::instance().starDriveModules()[highestID].arrival(distance));
                ship->setDelayTurns(Game::Technology::instance().starDriveModules()[highestID].delayTurns());
            }
            ship->setMovement(0);
            */
 /*       }
    }
    else
    {*/
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
                movementMap[ship->sector()->nextSectorInPath(ship->destination().sector())].push_back(ship);
                ship->sector()->removeShip(ship);
                if (ship->movement() >= 1)
                {
                    ship->setMovement(ship->movement() - 1);
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
/*

        bool first = true;
        bool canStillMove = true;
        while (canStillMove)
        {
            mCurrent = mCurrent->nextSectorInPath(mDestination);
            if (mCurrent != NULL)
            {
                if (!first)
                {
                    sleep500();
                }
                first = false;
                std::vector<Ship *> movedShips;
                for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
                {
                    Ship * ship = *it;
                    ship->setDelayTurns(0);
                    ship->setArrival(0);
                    ship->setDestination(SectorReference(mDestination));
                    if (ship->movement() >= 1 && ship->canMoveTo(mDestination))
                    {
                        //the following two lines modify variable mSector
                        ship->sector()->removeShip(ship);
                        if (ship->movement() >= 1)
                        {
                            ship->setMovement(ship->movement() - 1);
                        }
                        movedShips.push_back(ship);
                        if (mCurrent == mDestination)
                        {
                            ship->setDestination(Game::SectorReference());
                        }
                    }
                }
                mCurrent->addShips(movedShips, true); 
            }
      //  }
    }
    */
    mShips.clear();

/*
    for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
    {
        Ship * ship = *it;
        ship->setDelayTurns(0);
        ship->setArrival(0);
        ship->setDestination(SectorReference(mDestination));

        if (mCurrent->starSystem() != mDestination->starSystem())
        {
            int highestID = ship->config().highestID(Game::Component::StarDrive);
            if (highestID >= 0 && highestID < static_cast<int>(Game::Technology::instance().starDriveModules().size()))
            {
                float distance = ship->sector()->starSystem()->distance(mDestination->starSystem());
                ship->setArrival(Game::Technology::instance().starDriveModules()[highestID].arrival(distance));
                ship->setDelayTurns(Game::Technology::instance().starDriveModules()[highestID].delayTurns());
            }
            ship->setMovement(0);
        }
    }

    if (mCurrent->starSystem() == mDestination->starSystem())
    {
        bool canMove = true;
        while (canMove)
        {
            for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
            {
                if (ship->movement() < 1 || !ship->canMoveTo(mDestination))
                {
                    canMove = false;
                    break;
                }
            }
            if (!canMove)
            {
                break;
            }
            Game::Sector * next = mCurrent->nextSectorInPath(mDestination);
            if (next != NULL)
            {
                for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
                {
                    if (ship->movement() >= 1 && ship->canMoveTo(mDestination))
                    {

                    }
                }
                mCurrent = next;
            }
        }
    }
*/
/*
    for (std::vector<Ship *>::const_iterator it = mShips.begin(); it != mShips.end(); ++it)
    {
        Ship * ship = *it;
        ship->setDelayTurns(0);
        ship->setArrival(0);
        ship->setDestination(SectorReference(mDestination));
        if (ship->sector()->starSystem() != mDestination->starSystem())
        {
            int highestID = ship->config().highestID(Game::Component::StarDrive);
            if (highestID >= 0 && highestID < static_cast<int>(Game::Technology::instance().starDriveModules().size()))
            {
                float distance = ship->sector()->starSystem()->distance(mDestination->starSystem());
                ship->setArrival(Game::Technology::instance().starDriveModules()[highestID].arrival(distance));
                ship->setDelayTurns(Game::Technology::instance().starDriveModules()[highestID].delayTurns());
            }
            ship->setMovement(0);
        }
        else if (ship->movement() >= 1)
        {
            bool first = true;
            while (ship->canMoveTo(mDestination))
            {
                if (!first)
                {
                    first = false;
                    sleep500();
                }
                Game::Sector * next = ship->sector()->nextSectorInPath(mDestination);
                if (next != NULL)
                {
                    //the following two lines modify variable mSector
                    ship->sector()->removeShip(ship);
                    if (ship->movement() >= 1)
                    {
                        ship->setMovement(ship->movement() - 1);
                    }
                    next->addShip(ship, true); 
                    if (next == mDestination)
                    {
                        ship->setDestination(Game::SectorReference());
                    }
                }
            }
        }
    }*/
}
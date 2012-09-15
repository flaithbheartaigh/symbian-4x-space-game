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

#include "Sector.h"
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
#include "Warp.h"
#include "Ship.h"
#include "Universe.h"
#include "UniverseVisitor.h"
#include "SectorReference.h"
#include "Player.h"
#include "SectorCombat.h"
#include <string>
#include <algorithm>

using namespace Game;

namespace
{
    int direction(int number)
    {
        if (number == 0)
        {
            return 0;
        }
        else if (number > 0)
        {
            return 1;
        }
        return -1;
    }
}

Sector::Subscriber::~Subscriber()
{
    unsubscribe();
}

Sector::Subscriber::Subscriber(Sector * sector)
    : mSector(sector)
{
    if (mSector != NULL)
    {
        mSector->subscribe(this);
    }
}

void Sector::Subscriber::shipAdded(Ship *)
{

}

void Sector::Subscriber::shipRemoved(Ship *)
{

}

void Sector::Subscriber::planetSet(Planet *)
{

}

void Sector::Subscriber::planetUnset(Planet *)
{

}

void Sector::Subscriber::warpSet(Warp *)
{

}

void Sector::Subscriber::warpUnset(Warp *)
{

}

void Sector::Subscriber::starSet(Star *)
{

}

void Sector::Subscriber::starUnset(Star *)
{

}

void Sector::Subscriber::shipInTransitAdded(Ship *)
{

}

void Sector::Subscriber::shipInTransitRemoved(Ship *)
{

}

void Sector::Subscriber::selected()
{

}

void Sector::Subscriber::deselected()
{

}

void Sector::Subscriber::contentsChanged(bool)
{

}

Sector * Sector::Subscriber::sector() const
{
    return mSector;
}

void Sector::Subscriber::notify()
{
    if (mSector != NULL)
    {
        if (mSector->starSystem() != NULL && mSector->starSystem()->universe() != NULL && !mSector->starSystem()->universe()->notificationsBlocked())
        {
            bool changed = false;
            std::vector<Ship *> ships = mSector->ships();
            for (std::vector<Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
            {
                shipAdded(*it);
                if (!changed)
                {
                    changed = true;
                }
            }
            if (mSector->planet() != NULL)
            {
                planetSet(mSector->planet());
                if (!changed)
                {
                    changed = true;
                }
            }
            if (mSector->warp() != NULL)
            {
                warpSet(mSector->warp());
                if (!changed)
                {
                    changed = true;
                }
            }
            if (mSector->star() != NULL)
            {
                starSet(mSector->star());
                if (!changed)
                {
                    changed = true;
                }
            }
            if (!changed)
            {
                contentsChanged();
            }
        }
    }
}

void Sector::Subscriber::unsubscribe()
{
    if (mSector != NULL)
    {
        mSector->unsubscribe(this);
        mSector = NULL;
    }
}

Sector::~Sector()
{
    std::vector<Ship *> shipsInTransit = mShipsInTransit;
    for (std::vector<Ship *>::const_iterator it = shipsInTransit.begin(); it != shipsInTransit.end(); ++it)
    {
        removeShipFromTransit(*it);
        delete *it;
    }
    std::vector<Ship *> ships = mShips;
    for (std::vector<Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
    {
        removeShip(*it);
        delete *it;
    }
    setWarp(NULL);
    setPlanet(NULL);
    setStar(NULL);
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
}

Sector::Sector()
    : mStarSystem(NULL)
    , mX(0)
    , mY(0)
    , mStar(NULL)
    , mPlanet(NULL)
    , mWarp(NULL)
    , mShips()
    , mShipsInTransit()
    , mSubscribers()
{

}

Sector::Sector(StarSystem * starSystem)
    : mStarSystem(starSystem)
    , mX(0)
    , mY(0)
    , mStar(NULL)
    , mPlanet(NULL)
    , mWarp(NULL)
    , mShips()
    , mShipsInTransit()
    , mSubscribers()
{

}

StarSystem * Sector::starSystem() const
{
    return mStarSystem;
}

void Sector::setStarSystem(StarSystem * starSystem)
{
    mStarSystem = starSystem;
}

int Sector::x() const
{
    return mX;
}

void Sector::setX(int x)
{
    mX = x;
}

int Sector::y() const
{
    return mY;
}

void Sector::setY(int y)
{
    mY = y;
}

Star * Sector::star() const
{
    return mStar;
}

void Sector::setStar(Star * star)
{
    if (star != mStar)
    {
        bool changed = false;
        if (mStar != NULL)
        {
            changed = true;
            mStar->setSector(NULL);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->starUnset(mStar);
                }
            }
            delete mStar;
        }
        mStar = star;
        if (mStar != NULL)
        {
            changed = true;
            mStar->setSector(this);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->starSet(mStar);
                }
            }
        }
        if (changed)
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->contentsChanged();
            }
        }
    }
}

Planet * Sector::planet() const
{
    return mPlanet;
}

void Sector::setPlanet(Planet * planet)
{
    if (planet != mPlanet)
    {
        bool changed = false;
        if (mPlanet != NULL)
        {
            changed = true;
            mPlanet->setSector(NULL);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->planetUnset(mPlanet);
                }
            }
            delete mPlanet;
        }
        mPlanet = planet;
        if (mPlanet != NULL)
        {
            changed = true;
            mPlanet->setSector(this);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->planetSet(mPlanet);
                }
            }
        }
        if (changed)
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->contentsChanged();
            }
        }
    }
}

Warp * Sector::warp() const
{
    return mWarp;
}

void Sector::setWarp(Warp * warp)
{
    if (warp != mWarp)
    {
        bool changed = false;
        if (mWarp != NULL)
        {
            changed = true;
            mWarp->setSector(NULL);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->warpUnset(mWarp);
                }
            }
            delete mWarp;
        }
        mWarp = warp;
        if (mWarp != NULL)
        {
            changed = true;
            mWarp->setSector(this);
            if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
            {
                std::set<Subscriber *> subscribers = mSubscribers;
                for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
                {
                    (*it)->warpSet(mWarp);
                }
            }
        }
        if (changed)
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->contentsChanged();
            }
        }
    }
}

const std::vector<Ship *> & Sector::ships() const
{
    return mShips;
}

void Sector::addShips(const std::vector<Ship *> & ships)
{
    addShips(ships, false);
}

void Sector::addShips(const std::vector<Ship *> & ships, bool forceRedraw)
{
    for (std::vector<Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
    {
        _addShip(*it);
    }
    if (!ships.empty())
    {
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            SectorCombat combat(this);
            if (combat.canRun())
            {
                combat.run();
            }
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->contentsChanged(forceRedraw);
            }
        }
    }
}

void Sector::addShip(Ship * ship, bool forceRedraw)
{
    _addShip(ship);
    if (ship != NULL)
    {
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            SectorCombat combat(this);
            if (combat.canRun())
            {
                combat.run();
            }
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->contentsChanged(forceRedraw);
            }
        }
    }
}

void Sector::removeShip(Ship * ship, bool forceRedraw)
{
    if (ship != NULL)
    {
        ship->setSector(NULL);
        mShips.erase(std::remove(mShips.begin(), mShips.end(), ship), mShips.end());
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->shipRemoved(ship);
                (*it)->contentsChanged(forceRedraw);
            }
        }
    }
}

const std::vector<Ship *> & Sector::shipsInTransit() const
{
    return mShipsInTransit;
}

void Sector::addShipsInTransit(const std::vector<Ship *> & shipsInTransit)
{
    for (std::vector<Ship *>::const_iterator it = shipsInTransit.begin(); it != shipsInTransit.end(); ++it)
    {
        addShipInTransit(*it);
    }
}

void Sector::addShipInTransit(Ship * ship)
{
    if (ship != NULL)
    {
        ship->setInTransit(true);
        ship->setSector(this);
        mShipsInTransit.push_back(ship);
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->shipInTransitAdded(ship);
                (*it)->contentsChanged();
            }
        }
    }
}

void Sector::removeShipFromTransit(Ship * ship)
{
    if (ship != NULL)
    {
        ship->setSector(NULL);
        mShipsInTransit.erase(std::remove(mShipsInTransit.begin(), mShipsInTransit.end(), ship), mShipsInTransit.end());
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->shipInTransitRemoved(ship);
                (*it)->contentsChanged();
            }
        }
    }
}

std::set<Player *> Sector::players() const
{
    std::set<Player *> players;
    if (mPlanet != NULL && mPlanet->player() != NULL)
    {
        players.insert(mPlanet->player());
    }
    for (std::vector<Ship *>::const_iterator sIt = mShips.begin(); sIt != mShips.end(); ++sIt)
    {   
        if ((*sIt)->player() != NULL)
        {
            players.insert((*sIt)->player());
        }
    }
    return players;
}

void Sector::notifyChanged()
{
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->contentsChanged();
    }
}

void Sector::notifySelected()
{
    if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
    {
        std::set<Subscriber *> subscribers = mSubscribers;
        for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->selected();
        }
    }
}

void Sector::notifyDeselected()
{
    if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
    {
        std::set<Subscriber *> subscribers = mSubscribers;
        for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->deselected();
        }
    }
}

void Sector::subscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void Sector::unsubscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

void Sector::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
        if (mStar != NULL)
        {
            mStar->accept(visitor);
        }
        if (mPlanet != NULL)
        {
            mPlanet->accept(visitor);
        }
        if (mWarp != NULL)
        {
            mWarp->accept(visitor);
        }
        std::vector<Ship *> ships = mShips;
        for (std::vector<Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
        {
            if (*it != NULL)
            {
                (*it)->accept(visitor);
            }
        }
        std::vector<Ship *> shipsInTransit = mShipsInTransit;
        for (std::vector<Ship *>::const_iterator it = shipsInTransit.begin(); it != shipsInTransit.end(); ++it)
        {
            if (*it != NULL)
            {
                (*it)->accept(visitor);
            }
        }
    }
}

bool Sector::isEmpty() const
{
    return mShipsInTransit.empty() && mShips.empty() && mPlanet == NULL && mStar == NULL && mWarp == NULL;
}

Sector * Sector::nextSectorInPath(Sector * sector) const
{
    if (warp() != NULL && warp()->destination() == SectorReference(sector))
    {
        return sector;
    }

    if (sector == NULL || sector->starSystem() != mStarSystem)
    {
        return NULL;
    }

    int dx = sector->x() - mX;
    int dy = sector->y() - mY;

    return SectorReference(mStarSystem, mX + direction(dx), mY + direction(dy)).sector();
}

void Sector::_addShip(Ship * ship)
{
    if (ship != NULL)
    {
        ship->setInTransit(false);
        ship->setSector(this);
        mShips.push_back(ship);
        if (ship->player() != NULL)
        {
            ship->player()->addKnownSystem(StarSystemReference(starSystem()));
        }
        if (mStarSystem != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->shipAdded(ship);
            }
        }
    }
}

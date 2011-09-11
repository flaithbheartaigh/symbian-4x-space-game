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

#include "Shipyard.h"
#include "Planet.h"
#include "Ship.h"
#include "UniverseVisitor.h"

#include <algorithm>

using namespace Game;

Shipyard::~Shipyard()
{

}

Shipyard::Shipyard(Planet * planet)
    : Sector::Subscriber(planet != NULL ? planet->sector() : NULL)
    , mPlanet(planet)
    , mShipQueue()
{
    notify();
}

Planet * Shipyard::planet() const
{
    return mPlanet;
}

const std::vector<Ship *> & Shipyard::shipQueue() const
{
    return mShipQueue;
}

void Shipyard::addShip(Ship * ship)
{
    if (ship != NULL && ship->damage() > 0)
    {
        if (std::find(mShipQueue.begin(), mShipQueue.end(), ship) == mShipQueue.end())
        {
            mShipQueue.push_back(ship);
        }
    }
}

void Shipyard::removeShip(Ship * ship)
{
    if (ship != NULL)
    {
        mShipQueue.erase(std::remove(mShipQueue.begin(), mShipQueue.end(), ship), mShipQueue.end());
    }
}

void Shipyard::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}

void Shipyard::shipAdded(Ship * ship)
{
    if (ship != NULL)
    {
        addShip(ship);
    }
}

void Shipyard::shipRemoved(Ship * ship)
{
    if (ship != NULL)
    {
        removeShip(ship);
    }
}
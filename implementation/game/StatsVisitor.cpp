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

#include "StatsVisitor.h"

#include "Parameters.h"
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

StatsVisitor::~StatsVisitor()
{
    
}

StatsVisitor::StatsVisitor(Player * player)
    : UniverseVisitor()
    , mPlayer(player)
    , mPlanets()
    , mShips()
    , mShipyards()
    , mPopulation()
{
    
}

void StatsVisitor::visit(Universe * universe)
{
    if (universe != NULL)
    {

    }
}

void StatsVisitor::visit(StarSystem * starSystem)
{
    if (starSystem != NULL)
    {

    }
}

void StatsVisitor::visit(Sector * sector)
{
    if (sector != NULL)
    {

    }
}

void StatsVisitor::visit(Star * star)
{
    if (star != NULL)
    {

    }
}

void StatsVisitor::visit(Planet * planet)
{
    if (planet != NULL)
    {
        if (planet->player() == mPlayer)
        {
            mPlanets.push_back(planet);
            mPopulation += static_cast<int>(planet->population());
        }
    }
}

void StatsVisitor::visit(Ship * ship)
{
    if (ship != NULL)
    {
        if (ship->player() == mPlayer)
        {
            mShips.push_back(ship);
        }
    }
}

void StatsVisitor::visit(Shipyard * shipyard)
{
    if (shipyard != NULL)
    {
        if (shipyard->planet()->player() == mPlayer)
        {
            mShipyards.push_back(shipyard);
        }
    }
}

void StatsVisitor::visit(Player * player)
{
    if (player != NULL)
    {

    }
}
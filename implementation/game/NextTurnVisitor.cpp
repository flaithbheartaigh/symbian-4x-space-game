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

#include "NextTurnVisitor.h"
#include "StatsVisitor.h"

#include "Technology.h"
#include "Universe.h"
#include "Player.h"
#include "StarSystem.h"
#include "Sector.h"
#include "Star.h"
#include "Planet.h"
#include "Ship.h"
#include "ShipYard.h"

using namespace Game;

NextTurnVisitor::~NextTurnVisitor()
{
    
}

NextTurnVisitor::NextTurnVisitor()
    : UniverseVisitor()
{

}

void NextTurnVisitor::visit(Universe * universe)
{
    universe->setCurrentTurn(universe->currentTurn() + 1);
}

void NextTurnVisitor::visit(StarSystem * starSystem)
{

}

void NextTurnVisitor::visit(Sector * sector)
{
    for (std::vector<Ship *>::const_iterator it = sector->ships().begin(); it != sector->ships().end(); ++it)
    {

    }
}

void NextTurnVisitor::visit(Star * star)
{

}

void NextTurnVisitor::visit(Planet * planet)
{
    planet->setPopulation(planet->population() * 1.03f);
    if (planet->shipyard() == NULL && planet->population() >= 50.0f)
    {
        planet->setShipyard(new Shipyard(planet));
    }
    if (planet->population() >= 5000.0f)
    {
        planet->setPopulation(5000.0f);
    }
}

void NextTurnVisitor::visit(Ship * ship)
{
    ship->setMovement(ship->config().maximumMovement());
}

void NextTurnVisitor::visit(Shipyard * shipyard)
{
    std::vector<Ship *> shipQueue = shipyard->shipQueue();
    if (!shipQueue.empty())
    {
        Ship * ship = shipQueue.front();
        ship->repair(1);
        if (ship->damage() <= 0)
        {
            shipyard->removeShip(ship);
        }
    }
}

void NextTurnVisitor::visit(Player * player)
{
    player->setMoney(player->money() + player->revenue());
}
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

#include "Parameters.h"
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

}

void NextTurnVisitor::visit(Star * star)
{

}

void NextTurnVisitor::visit(Planet * planet)
{
    planet->setPopulation(planet->population() * 1.1f);
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
    if (ship->destination().isValid())
    {
        bool becameReady = false;
        bool justArrived = false;
        if (ship->delayTurns() >= 1)
        {
            ship->setDelayTurns(ship->delayTurns() - 1);
            becameReady = ship->delayTurns() < 1;
        }
        else if (ship->arrivalTurns() >= 1)
        {
            ship->setArrivalTurns(ship->arrivalTurns() - 1);
            justArrived = ship->arrivalTurns() < 1;
        }
        if (becameReady)
        {
            ship->sector()->removeShip(ship);
            ship->destination().sector()->addShipInTransit(ship); 
        }
        if (justArrived)
        {
            ship->destination().sector()->removeShipFromTransit(ship); 
            ship->destination().sector()->addShip(ship);
            ship->setDelayTurns(0);
            ship->setArrivalTurns(0);
            ship->setDestination(SectorReference());
            ship->setMovement(ship->maximumMovement());
        }
        else if (!ship->isInTransit() && ship->destination().sector()->starSystem() == ship->sector()->starSystem())
        {
            ship->moveTo(ship->destination().sector());
        }
        else
        {
            ship->setMovement(0);
        }
    }
    else
    {
        ship->setMovement(ship->maximumMovement());
    }
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
    if (player != NULL)
    {
        Game::StatsVisitor stats(player);
        Game::Universe::instance().accept(&stats);        
        for (unsigned int i = 0; i < stats.mPlanets.size(); ++i)
        {
            if (stats.mPlanets[i] != NULL)
            {
                player->setMoney(player->money() + int(stats.mPlanets[i]->population() / 10));
            }
        }
    }
}
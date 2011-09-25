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

#include "NextPlayerVisitor.h"

#include "Parameters.h"
#include "Universe.h"
#include "Player.h"
#include "AI.h"
#include "StarSystem.h"
#include "Sector.h"
#include "Star.h"
#include "Planet.h"
#include "Ship.h"
#include "ShipYard.h"

using namespace Game;

NextPlayerVisitor::~NextPlayerVisitor()
{
    
}

NextPlayerVisitor::NextPlayerVisitor()
    : UniverseVisitor()
{

}

void NextPlayerVisitor::visit(Universe * universe)
{
    unsigned int currentPlayerIndex = universe->game().currentPlayerIndex();
    ++currentPlayerIndex;
    if (currentPlayerIndex >= universe->game().players().size())
    {
        currentPlayerIndex = 0;
    }
    if (universe->game().players().size() > currentPlayerIndex)
    {
        universe->game().setCurrentPlayerIndex(currentPlayerIndex);
        if (universe->game().players()[currentPlayerIndex]->ai() != NULL)
        {
            universe->game().players()[currentPlayerIndex]->ai()->run();
        }
    }
}

void NextPlayerVisitor::visit(StarSystem * starSystem)
{

}

void NextPlayerVisitor::visit(Sector * sector)
{

}

void NextPlayerVisitor::visit(Star * star)
{

}

void NextPlayerVisitor::visit(Planet * planet)
{

}

void NextPlayerVisitor::visit(Ship * ship)
{

}

void NextPlayerVisitor::visit(Shipyard * shipyard)
{

}

void NextPlayerVisitor::visit(Player * player)
{

}

void NextPlayerVisitor::visit(StarDriveModule * starDriveModule)
{

}

void NextPlayerVisitor::visit(EngineModule * engineModule)
{

}

void NextPlayerVisitor::visit(ColonyModule * colonyModule)
{

}
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

#include "AI.h"
#include "Player.h"
#include "ShipConfig.h"
#include "Ship.h"
#include "Shipyard.h"
#include "Planet.h"
#include "Sector.h"
#include "StarSystem.h"
#include "Parameters.h"
#include "StatsVisitor.h"

#include <vector>
#include <map>

namespace
{
    Game::Component component(const std::vector<Game::Component> & components, Game::Component::Type type, unsigned int level) 
    {
        for (std::vector<Game::Component>::const_iterator it = components.begin(); it != components.end(); ++it)
        {
            if ((*it).type() == type && (*it).level() == level)
            {
                return (*it);
            }
        }
        return Game::Component();
    }

    std::multimap<float, Game::StarSystem *> distances(Game::StarSystem * start) 
    {
        std::multimap<float, Game::StarSystem *> systems;
        for (std::vector<Game::StarSystem *>::const_iterator it = Game::Universe::instance().starSystems().begin(); it != Game::Universe::instance().starSystems().end(); ++it)
        {
            systems.insert(std::pair<float, Game::StarSystem *>((*it)->distance(start), *it));
        }
        return systems;
    }
}

using namespace Game;

AI::~AI()
{

}

AI::AI()
    : mPlayer(NULL)
{

}

AI::AI(Player * player)
    : mPlayer(player)
{

}

Player * AI::player() const
{
    return mPlayer;
}

void AI::setPlayer(Player * player)
{
    mPlayer = player;
}

void AI::run()
{
    ShipConfig cheapExplorer;
    ShipConfig bestExplorer;
    ShipConfig cheapColony;
    ShipConfig bestColony;

    ShipConfig config;
    config.setComponents(mPlayer->components());

    Component lowEngine = component(mPlayer->components(), Component::Engine, config.lowestLevel(Component::Engine));
    Component highEngine = component(mPlayer->components(), Component::Engine, config.highestLevel(Component::Engine));
    Component highStarDrive = component(mPlayer->components(), Component::StarDrive, config.highestLevel(Component::StarDrive));
    Component lowColony = component(mPlayer->components(), Component::Colony, config.lowestLevel(Component::Colony));
    Component highColony = component(mPlayer->components(), Component::Colony, config.highestLevel(Component::Colony));

    if (cheapExplorer.components().empty())
    {
        cheapExplorer.setName("CheapExplorer");
        cheapExplorer.components().clear();
        cheapExplorer.components().push_back(lowEngine);
        cheapExplorer.components().push_back(highStarDrive);
    }

    if (cheapColony.components().empty())
    {
        cheapColony.setName("CheapColony");
        cheapColony.components().clear();
        cheapColony.components().push_back(lowEngine);
        cheapColony.components().push_back(highStarDrive);
        cheapColony.components().push_back(lowColony);
    }

    StatsVisitor visitor(mPlayer);
    Game::Universe::instance().accept(&visitor);
    for (std::vector<Shipyard *>::iterator it = visitor.mShipyards.begin(); it != visitor.mShipyards.end(); ++it)
    {
        if (*it != NULL && (*it)->shipQueue().size() < 2)
        {
            if (visitor.mShips.size() >= 2)
            {
                if (cheapColony.cost() <= mPlayer->money())
                {
                    mPlayer->setMoney(mPlayer->money() - cheapColony.cost());
                    Game::Ship * ship = new Game::Ship((*it)->planet()->sector(), cheapColony);
                    ship->setPlayer(mPlayer);
                    (*it)->planet()->sector()->addShip(ship);
                }
            }
            else if (cheapExplorer.cost() <= mPlayer->money())
            {
                mPlayer->setMoney(mPlayer->money() - cheapExplorer.cost());
                Game::Ship * ship = new Game::Ship((*it)->planet()->sector(), cheapExplorer);
                ship->setPlayer(mPlayer);
                (*it)->planet()->sector()->addShip(ship);
            }
        }
    }
    for (std::vector<Ship *>::iterator it = visitor.mShips.begin(); it != visitor.mShips.end(); ++it)
    {
        bool somethingApplies = false;
        if ((*it)->damage() > 0)
        {
            //repair(ship);
        }
        else if (!(*it)->destination().isValid())
        {
            if ((*it)->config().has(Component::Colony) && (*it)->population() > 0.0)
            {
                if (((*it)->sector()->planets().size() > 0 && (*it)->sector()->planets()[0]->player() == NULL))
                {
                    if ((*it)->canColonize())
                    {
                        (*it)->colonize();
                        somethingApplies = true;
                        break;
                    }
                }
                else
                {
                    std::multimap<float, Game::StarSystem *> sorted = distances((*it)->sector()->starSystem());
                    for (std::multimap<float, Game::StarSystem *>::iterator it2 = sorted.begin(); it2 != sorted.end(); ++it2)
                    {
                        if (mPlayer->knows((*it2).second)) //?
                        {
                            for (std::vector<Sector *>::const_iterator sit = (*it2).second->sectors().begin(); sit != (*it2).second->sectors().end(); ++sit)
                            {
                                if ((*sit)->planets().size() > 0 && (*sit)->planets()[0]->player() == NULL)
                                {
                                    if ((*it)->canMoveTo(*sit))
                                    {
                                        (*it)->moveTo(*sit);
                                        somethingApplies = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }   
                }
            }

            if (!somethingApplies)
            {
                std::multimap<float, Game::StarSystem *> sorted = distances((*it)->sector()->starSystem());
                for (std::multimap<float, Game::StarSystem *>::iterator it2 = sorted.begin(); it2 != sorted.end(); ++it2)
                {
                    if (!mPlayer->knows((*it2).second))
                    {
                        if ((*it)->canMoveTo(SectorReference((*it2).second, 1, 1).sector()))
                        {
                            (*it)->moveTo(SectorReference((*it2).second, 1, 1).sector());
                            break;
                        }
                    }
                }
            }
        }
    }
}
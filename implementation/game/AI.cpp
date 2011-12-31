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
#include "Technology.h"
#include "StatsVisitor.h"
#include "ShipMovement.h"

#include <vector>
#include <map>

namespace
{
    Game::Component bestComponent(const std::vector<Game::Component> & components, Game::Component::Type type) 
    {
        Game::Component component;
        for (std::vector<Game::Component>::const_reverse_iterator it = components.rbegin(); it != components.rend(); ++it)
        {
            if ((*it).type() == type)
            {
                component = (*it);
            }
        }
        return component;
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

    bool canColonizeLocal(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component colony = bestComponent(player->components(), Game::Component::Colony);

        return engine.type() != Game::Component::None && colony.type() != Game::Component::None;
    }

    bool canColonizeRemote(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component colony = bestComponent(player->components(), Game::Component::Colony);
        Game::Component starDrive = bestComponent(player->components(), Game::Component::StarDrive);

        return engine.type() != Game::Component::None && colony.type() != Game::Component::None && starDrive.type() != Game::Component::None;
    }

    bool canExplore(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component starDrive = bestComponent(player->components(), Game::Component::StarDrive);

        return engine.type() != Game::Component::None && starDrive.type() != Game::Component::None;
    }

    Game::ShipConfig colonyLocal(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component colony = bestComponent(player->components(), Game::Component::Colony);

        Game::ShipConfig config;
        config.setName("Colony");
        config.components().push_back(engine);
        config.components().push_back(colony);

        return config;
    }

    Game::ShipConfig colonyRemote(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component colony = bestComponent(player->components(), Game::Component::Colony);
        Game::Component starDrive = bestComponent(player->components(), Game::Component::StarDrive);

        Game::ShipConfig config;
        config.setName("Colony X");
        config.components().push_back(engine);
        config.components().push_back(colony);
        config.components().push_back(starDrive);

        return config;
    }

    Game::ShipConfig explorer(Game::Player * player)
    {
        Game::Component engine = bestComponent(player->components(), Game::Component::Engine);
        Game::Component starDrive = bestComponent(player->components(), Game::Component::StarDrive);

        Game::ShipConfig config;
        config.setName("Explorer");
        config.components().push_back(engine);
        config.components().push_back(starDrive);

        return config;
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
    StatsVisitor visitor(player());
    Game::Universe::instance().accept(&visitor);
    std::map<Sector *, std::vector<Ship *> > ships;
    for (std::vector<Ship *>::iterator it = visitor.mShips.begin(); it != visitor.mShips.end(); ++it)
    {
        ships[(*it)->sector()].push_back(*it);
    }
    for (std::map<Sector *, std::vector<Ship *> >::const_iterator it = ships.begin(); it != ships.end(); ++it)
    {
        ShipMovement shipMovement;
        shipMovement.addShips((*it).second);
        shipMovement.run();
    }
}

NPC::~NPC()
{

}

NPC::NPC()
    : AI(NULL)
{

}

NPC::NPC(Player * player)
    : AI(player)
{

}

void NPC::run()
{
    AI::run();

    ShipConfig explorer = ::explorer(player());
    ShipConfig colony = ::colonyRemote(player());

    StatsVisitor visitor(player());
    Game::Universe::instance().accept(&visitor);
    for (std::vector<Shipyard *>::iterator it = visitor.mShipyards.begin(); it != visitor.mShipyards.end(); ++it)
    {
        if (*it != NULL && (*it)->shipQueue().size() < 2)
        {
            if (player()->revenue() < colony.cost())
            {
                if (colony.cost() <= player()->money())
                {
                    player()->setMoney(player()->money() - colony.cost());
                    Game::Ship * ship = new Game::Ship((*it)->planet()->sector(), colony);
                    ship->setPlayer(player());
                    (*it)->planet()->sector()->addShip(ship);
                }
            }
            else if (explorer.cost() <= player()->money())
            {
                player()->setMoney(player()->money() - explorer.cost());
                Game::Ship * ship = new Game::Ship((*it)->planet()->sector(), explorer);
                ship->setPlayer(player());
                (*it)->planet()->sector()->addShip(ship);
            }
        }
    }

    Game::ShipMovement shipMovement;
    for (std::vector<Ship *>::iterator it = visitor.mShips.begin(); it != visitor.mShips.end(); ++it)
    {
        bool somethingApplies = false;
        if ((*it)->config().has(Component::Colony) && (*it)->population() > 0.0)
        {
            if ((*it)->canColonize())
            {
                (*it)->colonize();
                somethingApplies = true;
            }
        }
        if ((*it)->damage() > 0)
        {
            //repair(ship);
            //somethingApplies = true;
        }
        else if (!(*it)->destination().isValid())
        {
            if ((*it)->config().has(Component::Colony) && (*it)->population() > 0.0)
            {
                {
                    std::multimap<float, Game::StarSystem *> sorted = distances((*it)->sector()->starSystem());
                    for (std::multimap<float, Game::StarSystem *>::iterator it2 = sorted.begin(); it2 != sorted.end(); ++it2)
                    {
                        if (player()->knows((*it2).second)) //?
                        {
                            for (std::vector<Sector *>::const_iterator sit = (*it2).second->sectors().begin(); sit != (*it2).second->sectors().end(); ++sit)
                            {
                                if ((*sit)->planet() != NULL && (*sit)->planet()->player() == NULL)
                                {
                                    if ((*it)->canMoveTo(*sit))
                                    {
                                        Game::ShipMovement shipDestination;
                                        shipDestination.addShip(*it);
                                        shipDestination.setDestination(*sit);

                                        shipMovement.addShip(*it);
                                        somethingApplies = true;
                                        break;
                                    }
                                }
                            }
                            if (somethingApplies)
                            {
                                break;
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
                    if (!player()->knows((*it2).second))
                    {
                        Sector * sector = SectorReference((*it2).second, 1, 1).sector();
                        if ((*it)->canMoveTo(sector))
                        {
                            Game::ShipMovement shipDestination;
                            shipDestination.addShip(*it);
                            shipDestination.setDestination(sector);

                            shipMovement.addShip(*it);
                            break;
                        }
                    }
                }
            }
        }
    }
    shipMovement.run();
}
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

#include "SectorCombat.h"
#include "Sector.h"
#include "Ship.h"
#include "ShipConfig.h"
#include "Technology.h"

#include <vector>
#include <cmath>
#include <ctime>

namespace
{
    std::vector<Game::Ship *> allExcept(const std::vector<Game::Ship *> & ships, Game::Player * player)
    {
        std::vector<Game::Ship *> except;
        for (std::vector<Game::Ship *>::const_iterator it = ships.begin(); it != ships.end(); ++it)
        {
            if ((*it)->player() != player && (*it)->hitPoints() > 0)
            {
                except.push_back(*it);
            }
        }
        return except;
    }

    Game::Ship * randomPick(const std::vector<Game::Ship *> & ships)
    {
        if (ships.size() > 0)
        {
            return ships[rand() % ships.size()];
        }
        return NULL;
    }
}

using namespace Game;

SectorCombat::~SectorCombat()
{

}

SectorCombat::SectorCombat(Sector * sector)
    : mSector(sector)
{
    srand(static_cast<unsigned int>(time(NULL)));
}

bool SectorCombat::canRun() const
{
    if (mSector != NULL && mSector->players().size() > 1)
    {
        for (std::vector<Ship *>::const_iterator it = mSector->ships().begin(); it != mSector->ships().end(); ++it)
        {
            if ((*it)->canFight())
            {
                return true;
            }
        }
    }
    return false;
}

void SectorCombat::run()
{
    for (int i = 0; i < 20; ++i)
    {
        std::vector<Ship *> destroyed;
        for (std::vector<Ship *>::const_reverse_iterator it = mSector->ships().rbegin(); it != mSector->ships().rend(); ++it)
        {
            Ship * ship = *it;
            if (ship->canFight())
            {
                Ship * enemy = randomPick(allExcept(mSector->ships(), ship->player()));
                if (enemy != NULL)
                {
                    std::vector<Component> weapons = ship->config().components(Component::Weapon);
                    for (std::vector<Component>::const_iterator itc = weapons.begin(); itc != weapons.end(); ++itc)
                    {
                        bool hit = rand() % (enemy->config().maximumMovement() + 1) == 0;
                        if (hit)
                        {
                            int damage = (rand() % static_cast<int>((*itc).value())/*Technology::instance().weaponModules()[(*itc).ID()].damage())*/ + 1);
                            enemy->damage(damage);
                            if (enemy->hitPoints() <= 0)
                            {
                                destroyed.push_back(enemy);
                            }
                        }
                    }
                }
            }
        }
        for (std::vector<Ship *>::const_iterator it = destroyed.begin(); it != destroyed.end(); ++it)
        {
            (*it)->sector()->removeShip((*it), true);
            delete (*it);
        }
    }
}

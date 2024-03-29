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

#include "Ship.h"
#include "Player.h"
#include "Universe.h"
#include "StarSystem.h"
#include "Sector.h"
#include "Planet.h"
#include "Warp.h"
#include "Component.h"
#include "Technology.h"
#include "UniverseVisitor.h"

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h> //Sleep
#else
    #include <unistd.h>  //sleep
#endif

#include <algorithm>

using namespace Game;

Ship::~Ship()
{

}

Ship::Ship()
    : mConfig()
    , mName("Unknown")
    , mSector(NULL)
    , mPlayerName()
    , mPlayer(NULL)
    , mInTransit(false)
    , mDestination(NULL)
    , mArrival(0)
    , mPopulation(0.0f)
    , mMovement(0)
{
    
}

Ship::Ship(Sector * sector, const ShipConfig & config)
    : mConfig(config)
    , mName("Unknown")
    , mSector(sector)
    , mPlayerName()
    , mPlayer(NULL)
    , mInTransit(false)
    , mDestination()
    , mArrival(0)
    , mPopulation(0.0f)
    , mMovement(0)
{
    initFromConfig(mConfig);
}

const ShipConfig & Ship::config() const
{
    return mConfig;
}

void Ship::setConfig(const ShipConfig & config)
{
    mConfig = config;
}

const std::string & Ship::name() const
{
    return mName;
}

void Ship::setName(const std::string & name)
{
    mName = name;
}

Player * Ship::player() const
{
    if (mPlayer == NULL && !mPlayerName.empty() && mSector != NULL && mSector->starSystem() != NULL && mSector->starSystem()->universe() != NULL)
    {
        return mSector->starSystem()->universe()->game().player(mPlayerName);
    }
    return mPlayer;
}

void Ship::setPlayer(Player * player)
{
    mPlayer = player;
    if (mPlayer != NULL)
    {
        mPlayerName = mPlayer->name();
    }
    else
    {
        mPlayerName = "";
    }
}

std::string Ship::playerName() const
{
    if (mPlayer != NULL)
    {
        return mPlayer->name();
    }
    return mPlayerName;
}

void Ship::setPlayerName(const std::string & playerName)
{
    mPlayerName = playerName;
    if (mSector != NULL && mSector->starSystem() != NULL && mSector->starSystem()->universe() != NULL)
    {
        mPlayer = mSector->starSystem()->universe()->game().player(mPlayerName);
    }
}

Sector * Ship::sector() const
{
    return mSector;
}

void Ship::setSector(Sector * sector)
{
    mSector = sector;
}

float Ship::population() const
{
    return mPopulation;
}

void Ship::setPopulation(float population)
{
    mPopulation = population;
}

unsigned int Ship::movement() const
{
    return mMovement;
}

void Ship::setMovement(unsigned int movement)
{
    mMovement = movement;
}

const std::vector<SectorReference> & Ship::destination() const
{
    return mDestination;
}

std::vector<SectorReference> & Ship::destination()
{
    return mDestination;
}

void Ship::setDestination(const std::vector<SectorReference> & destination)
{
    mDestination = destination;
}

unsigned int Ship::arrival() const
{
    return mArrival;
}

void Ship::setArrival(unsigned int arrival)
{
    mArrival = arrival;
}

bool Ship::canColonize() const
{
    bool ret = true;
    ret = ret && mConfig.has(Component::Colony) && population() > 0.0;
    ret = ret && !isInTransit() && mSector != NULL && mSector->planet() != NULL && mSector->planet()->player() == NULL;
    return ret;
}

void Ship::colonize()
{
    if (mSector != NULL)
    {
        Planet * planet = mSector->planet();
        if (planet != NULL)
        {
            planet->setPlayer(player());
            planet->setPopulation(planet->population() + mPopulation);
            mSector->notifyChanged();
            setPopulation(0.0f);
        }
    }
}

bool Ship::canUnload() const
{
    bool ret = true;
    ret = ret && mConfig.has(Component::Colony);
    ret = ret && !isInTransit() && mSector != NULL && mSector->planet() != NULL && mSector->planet()->player() == player();
    return ret;
}

void Ship::unload()
{
    if (mSector != NULL)
    {
        Planet * planet = mSector->planet();
        if (planet != NULL)
        {
            planet->setPopulation(planet->population() + mPopulation);
            setPopulation(0.0f);
            mSector->notifyChanged();
        }
    }
}

bool Ship::canLoad() const
{
    bool ret = true;
    ret = ret && mConfig.has(Component::Colony);
    ret = ret && !isInTransit() && mSector != NULL && mSector->planet() != NULL && mSector->planet()->player() == player();
    return ret;
}

void Ship::load()
{
    if (mSector != NULL)
    {
        Planet * planet = mSector->planet();
        if (planet != NULL)
        {
            float population = mConfig.maximumPopulation();
            population = std::max(0.0f, std::min(population - mPopulation, planet->population()));
            planet->setPopulation(planet->population() - population);
            setPopulation(population);
            mSector->notifyChanged();
        }
    }
}
/*
bool Ship::canMoveTo(Sector * sector) const
{
    bool ret = true;
    ret = ret && !isInTransit() && mArrival == 0;
    ret = ret && mMovement > 0;
    ret = ret && sector != NULL && sector != mSector;
    if (ret)
    {
        ret = ret && ((sector->starSystem() == mSector->starSystem()) 
            || (sector->starSystem() != mSector->starSystem() && mConfig.has(Component::StarDrive))
            || (mSector->warp() != NULL && mSector->warp()->destination() == SectorReference(sector)));
    }
    return ret;
}
*/
bool Ship::canFight() const
{
    bool ret = true;
    ret = ret && mConfig.has(Component::Weapon);
    ret = ret && !isInTransit() && mSector != NULL;
    return ret;
}

bool Ship::isInTransit() const
{
    return mInTransit;
}

void Ship::setInTransit(bool inTransit)
{
    mInTransit = inTransit;
}

unsigned int Ship::hitPoints() const
{
    unsigned int hitPoints = 0;
    for (std::vector<Component>::const_iterator it = mConfig.components().begin(); it != mConfig.components().end(); ++it)
    {
        hitPoints += (*it).hitPoints();
    }
    return hitPoints;
}

unsigned int Ship::damage() const
{
    unsigned int damage = 0;
    for (std::vector<Component>::const_iterator it = mConfig.components().begin(); it != mConfig.components().end(); ++it)
    {
        damage += (*it).damage();
    }
    return damage;
}

void Ship::repair(unsigned int hitPoints)
{
    for (std::vector<Component>::iterator it = mConfig.components().begin(); it != mConfig.components().end(); ++it)
    {
        if (hitPoints > 0 && (*it).damage() > 0)
        {
            if ((*it).damage() < hitPoints)
            {
                hitPoints -= (*it).damage();
                (*it).setDamage(0);
            }
            else
            {
                (*it).setDamage((*it).damage() - hitPoints);
                hitPoints = 0;
            }
        }
    }
    // repair colony
    mPopulation = mConfig.maximumPopulation();
}

void Ship::damage(unsigned int hitPoints)
{
    for (std::vector<Component>::iterator it = mConfig.components().begin(); it != mConfig.components().end(); ++it)
    {
        if (hitPoints > 0 && (*it).remain() > 0)
        {
            if ((*it).remain() < hitPoints)
            {
                hitPoints -= (*it).remain();
                (*it).setRemain(0);
            }
            else
            {
                (*it).setRemain((*it).remain() - hitPoints);
                hitPoints = 0;
            }
        }
    }

    if (mPopulation > mConfig.maximumPopulation())
    {
        mPopulation = mConfig.maximumPopulation();
    }
    if (mMovement > mConfig.maximumMovement())
    {
        mMovement = mConfig.maximumMovement();
    }
}

void Ship::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}

void Ship::initFromConfig(const ShipConfig & config)
{
    mName = config.name();
    damage(hitPoints());
    mMovement = config.maximumMovement();
}

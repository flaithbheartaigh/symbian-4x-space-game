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

#include "Technology.h"
#include "Universe.h"

#include <cmath>
#include <limits>

using namespace Game;
/*
Technology::ColonyModule::~ColonyModule()
{

}

Technology::ColonyModule::ColonyModule()
    : mComponent()
    , mPopulation(0.0f)
{

}

Technology::ColonyModule::ColonyModule(const ColonyModule & other)
    : mComponent(other.mComponent)
    , mPopulation(other.mPopulation)
{

}

Technology::ColonyModule & Technology::ColonyModule::operator =(const ColonyModule & other)
{
    if (&other != this)
    {
        mComponent = other.mComponent;
        mPopulation = other.mPopulation;
    }
    return *this;
}

float Technology::ColonyModule::population() const
{
    return mPopulation;
}

void Technology::ColonyModule::setPopulation(float population)
{
    mPopulation = population;
}


const Component & Technology::ColonyModule::component() const
{
    return mComponent;
}

Component & Technology::ColonyModule::component()
{
    return mComponent;
}

Technology::StarDriveModule::~StarDriveModule()
{

}

Technology::StarDriveModule::StarDriveModule()
    : mComponent()
    , mTravelSpeed(0.0f)
{

}

Technology::StarDriveModule::StarDriveModule(const StarDriveModule & other)
    : mComponent(other.mComponent)
    , mTravelSpeed(other.mTravelSpeed)
{

}

Technology::StarDriveModule & Technology::StarDriveModule::operator =(const StarDriveModule & other)
{
    if (&other != this)
    {
        mComponent = other.mComponent;
        mTravelSpeed = other.mTravelSpeed;
    }
    return *this;
}

float Technology::StarDriveModule::travelSpeed() const
{
    return mTravelSpeed;
}

void Technology::StarDriveModule::setTravelSpeed(float travelSpeed)
{
    mTravelSpeed = travelSpeed;
}

unsigned int Technology::StarDriveModule::arrival(float distance) const
{
    unsigned int turns = 0;
    float inverseOfSpeed = 1.0;
    if (mTravelSpeed > 0.0f)
    {
        inverseOfSpeed = inverseOfSpeed / mTravelSpeed;
    }
    float delay = pow(distance, inverseOfSpeed);
    if (delay < float(std::numeric_limits<unsigned int>::max()))
    {
        turns = static_cast<unsigned int>(delay);
    }
    else
    {
        turns = 99999;
    }
    if (turns > 99999)
    {
        turns = 99999;
    }
    return Game::Universe::instance().currentTurn() + turns;
}

const Component & Technology::StarDriveModule::component() const
{
    return mComponent;
}

Component & Technology::StarDriveModule::component()
{
    return mComponent;
}

Technology::EngineModule::~EngineModule()
{

}

Technology::EngineModule::EngineModule()
    : mComponent()
    , mSpeed(0)
{

}

Technology::EngineModule::EngineModule(const EngineModule & other)
    : mComponent(other.mComponent)
    , mSpeed(other.mSpeed)
{

}

Technology::EngineModule & Technology::EngineModule::operator =(const EngineModule & other)
{
    if (&other != this)
    {
        mComponent = other.mComponent;
        mSpeed = other.mSpeed;
    }
    return *this;
}

unsigned int Technology::EngineModule::speed() const
{
    return mSpeed;
}

void Technology::EngineModule::setSpeed(unsigned int speed)
{
    mSpeed = speed;
}

const Component & Technology::EngineModule::component() const
{
    return mComponent;
}

Component & Technology::EngineModule::component()
{
    return mComponent;
}

Technology::WeaponModule::~WeaponModule()
{

}

Technology::WeaponModule::WeaponModule()
    : mComponent()
    , mDamage(0)
{

}

Technology::WeaponModule::WeaponModule(const WeaponModule & other)
    : mComponent(other.mComponent)
    , mDamage(other.mDamage)
{

}

Technology::WeaponModule & Technology::WeaponModule::operator =(const WeaponModule & other)
{
    if (&other != this)
    {
        mComponent = other.mComponent;
        mDamage = other.mDamage;
    }
    return *this;
}

unsigned int Technology::WeaponModule::damage() const
{
    return mDamage;
}

void Technology::WeaponModule::setDamage(unsigned int weapon)
{
    mDamage = weapon;
}

const Component & Technology::WeaponModule::component() const
{
    return mComponent;
}

Component & Technology::WeaponModule::component()
{
    return mComponent;
}
*/
Technology * Technology::_instance = NULL;

Technology & Technology::instance()
{
    if (_instance == NULL)
    {
        _instance = new Technology(); 
    }
    return *_instance;
}

Technology::~Technology()
{
    _instance = NULL;
}
/*
const std::vector<Technology::ColonyModule> & Technology::colonyModules() const
{
    return mColonyModules;
}

void Technology::setColonyModules(const std::vector<ColonyModule> & colonyModules)
{
    mColonyModules = colonyModules;
}

const std::vector<Technology::StarDriveModule> & Technology::starDriveModules() const
{
    return mStarDriveModules;
}

void Technology::setStarDriveModules(const std::vector<StarDriveModule> & starDriveModules)
{
    mStarDriveModules = starDriveModules;
}

const std::vector<Technology::EngineModule> & Technology::engineModules() const
{
    return mEngineModules;
}

void Technology::setEngineModules(const std::vector<EngineModule> & engineModules)
{
    mEngineModules = engineModules;
}

const std::vector<Technology::WeaponModule> & Technology::weaponModules() const
{
    return mWeaponModules;
}

void Technology::setWeaponModules(const std::vector<WeaponModule> & weaponModules)
{
    mWeaponModules = weaponModules;
}
*/
const std::vector<Component> & Technology::components() const
{
    return mComponents;
}

void Technology::setComponents(const std::vector<Component> & components)
{
    mComponents = components;
}

Technology::Technology()
    : mComponents()
    /*mColonyModules()
    , mStarDriveModules()
    , mEngineModules()
    , mWeaponModules()*/
{

}
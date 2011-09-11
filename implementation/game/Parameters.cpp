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

#include "Parameters.h"

#include <cmath>
#include <limits>
#include <sstream>

using namespace Game;

Parameters::ColonyModule::~ColonyModule()
{

}

Parameters::ColonyModule::ColonyModule()
    : mPopulation(0.0f)
{

}

Parameters::ColonyModule::ColonyModule(float population)
    : mPopulation(population)
{

}

Parameters::ColonyModule::ColonyModule(const ColonyModule & other)
    : mPopulation(other.mPopulation)
{

}

Parameters::ColonyModule & Parameters::ColonyModule::operator =(const ColonyModule & other)
{
    if (&other != this)
    {
        mPopulation = other.mPopulation;
    }
    return *this;
}

float Parameters::ColonyModule::population() const
{
    return mPopulation;
}

void Parameters::ColonyModule::setPopulation(float population)
{
    mPopulation = population;
}

Parameters::StarDriveModule::~StarDriveModule()
{

}

Parameters::StarDriveModule::StarDriveModule()
    : mDelayTurns(0)
    , mTravelSpeed(0.0f)
{

}

Parameters::StarDriveModule::StarDriveModule(unsigned int delayTurns, float travelSpeed)
    : mDelayTurns(delayTurns)
    , mTravelSpeed(travelSpeed)
{

}

Parameters::StarDriveModule::StarDriveModule(const StarDriveModule & other)
    : mDelayTurns(other.mDelayTurns)
    , mTravelSpeed(other.mTravelSpeed)
{

}

Parameters::StarDriveModule & Parameters::StarDriveModule::operator =(const StarDriveModule & other)
{
    if (&other != this)
    {
        mDelayTurns = other.mDelayTurns;
        mTravelSpeed = other.mTravelSpeed;
    }
    return *this;
}

unsigned int Parameters::StarDriveModule::delayTurns() const
{
    return mDelayTurns;
}

void Parameters::StarDriveModule::setDelayTurns(unsigned int delayTurns)
{
    mDelayTurns = delayTurns;
}

float Parameters::StarDriveModule::travelSpeed() const
{
    return mTravelSpeed;
}

void Parameters::StarDriveModule::setTravelSpeed(float travelSpeed)
{
    mTravelSpeed = travelSpeed;
}

unsigned int Parameters::StarDriveModule::arrivalTurns(float distance) const
{
    unsigned int arrivalTurns = 0;
    float inverseOfSpeed = 1.0;
    if (mTravelSpeed > 0.0f)
    {
        inverseOfSpeed = inverseOfSpeed / mTravelSpeed;
    }
    float delay = pow(distance, inverseOfSpeed);
    if (delay < float(std::numeric_limits<unsigned int>::max()))
    {
        arrivalTurns = static_cast<unsigned int>(delay);
    }
    else
    {
        arrivalTurns = 99999;
    }
    if (arrivalTurns > 99999)
    {
        arrivalTurns = 99999;
    }
    return arrivalTurns;
}

Parameters::EngineModule::~EngineModule()
{

}

Parameters::EngineModule::EngineModule()
    : mSpeed(0)
{

}

Parameters::EngineModule::EngineModule(unsigned int speed)
    : mSpeed(speed)
{

}

Parameters::EngineModule::EngineModule(const EngineModule & other)
    : mSpeed(other.mSpeed)
{

}

Parameters::EngineModule & Parameters::EngineModule::operator =(const EngineModule & other)
{
    if (&other != this)
    {
        mSpeed = other.mSpeed;
    }
    return *this;
}

unsigned int Parameters::EngineModule::speed() const
{
    return mSpeed;
}

void Parameters::EngineModule::setSpeed(unsigned int speed)
{
    mSpeed = speed;
}

Parameters * Parameters::_instance = NULL;

Parameters & Parameters::instance()
{
    if (_instance == NULL)
    {
        _instance = new Parameters(); 
    }
    return *_instance;
}

Parameters::~Parameters()
{
    _instance = NULL;
}

const std::vector<Parameters::ColonyModule> & Parameters::colonyModules() const
{
    return mColonyModules;
}

void Parameters::setColonyModules(const std::vector<ColonyModule> & colonyModules)
{
    mColonyModules = colonyModules;
}

const std::vector<Parameters::StarDriveModule> & Parameters::starDriveModules() const
{
    return mStarDriveModules;
}

void Parameters::setStarDriveModules(const std::vector<StarDriveModule> & starDriveModules)
{
    mStarDriveModules = starDriveModules;
}

const std::vector<Parameters::EngineModule> & Parameters::engineModules() const
{
    return mEngineModules;
}

void Parameters::setEngineModules(const std::vector<EngineModule> & engineModules)
{
    mEngineModules = engineModules;
}

void Parameters::setDataFilesPath(const std::string & dataFilesPath)
{
    mDataFilesPath = dataFilesPath;
}

const std::string & Parameters::getDataFilesPath() const
{
    return mDataFilesPath;
}

std::string Parameters::getDataFilePath(const std::string & path) const
{
    std::stringstream ss;
    ss << mDataFilesPath << "/" << path;
    return ss.str();
}

Parameters::Parameters()
    : mColonyModules()
    , mStarDriveModules()
    , mEngineModules()
    , mDataFilesPath("../data")
{

}
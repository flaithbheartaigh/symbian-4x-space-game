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

#include "ShipConfig.h"
#include "Technology.h"

#include <string>

using namespace Game;

ShipConfig::~ShipConfig()
{
    
}

ShipConfig::ShipConfig()
    : mName()
    , mComponents()
{

}

ShipConfig::ShipConfig(const ShipConfig & other)
    : mName(other.mName)
    , mComponents(other.mComponents)
{
}

ShipConfig & ShipConfig::operator =(const ShipConfig & other)
{
    if (&other != this)
    {
        mName = other.mName;
        mComponents = other.mComponents;
    }
    return *this;
}

const std::string & ShipConfig::name() const
{
    return mName;
}

void ShipConfig::setName(const std::string & name)
{
    mName = name;
}

const std::vector<Component> & ShipConfig::components() const
{
    return mComponents;
}

std::vector<Component> & ShipConfig::components()
{
    return mComponents;
}

void ShipConfig::setComponents(const std::vector<Component> & components)
{
    mComponents = components;
}

bool ShipConfig::has(Component::Type type) const
{
    bool ret = false;
    for (std::vector<Component>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        if ((*it).type() == type && !(*it).destroyed())
        {
            ret = true;
            break;
        }
    }
    return ret;
}

std::vector<Component> ShipConfig::components(Component::Type type) const
{
    std::vector<Component> ret;
    for (std::vector<Component>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        if ((*it).type() == type && !(*it).destroyed())
        {
            ret.push_back(*it);
        }
    }
    return ret;
}

int ShipConfig::highestID(Component::Type type) const
{
    int ID = -1;
    for (std::vector<Component>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        if ((*it).type() == type && static_cast<int>((*it).ID()) > ID && !(*it).destroyed())
        {
            ID = (*it).ID();
        }
    }
    return ID;
}

int ShipConfig::lowestID(Component::Type type) const
{
    int ID = -1;
    for (std::vector<Component>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        if ((*it).type() == type && (ID < 0 || static_cast<int>((*it).ID()) < ID) && !(*it).destroyed())
        {
            ID = (*it).ID();
        }
    }
    return ID;
}

int ShipConfig::cost() const
{
    int cost = 0;
    for (std::vector<Component>::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it)
    {
        cost += (*it).cost();
    }
    return cost;
}

unsigned int ShipConfig::maximumMovement() const
{
    unsigned int maximum = 0;
    std::vector<Component> comps = components(Component::Engine);
    for (std::vector<Component>::const_iterator it = comps.begin(); it != comps.end(); ++it)
    {
        maximum += Technology::instance().engineModules()[(*it).ID()].speed();
    }
    return maximum;
}

float ShipConfig::maximumPopulation() const
{
    float maximum = 0.0f;
    std::vector<Component> comps = components(Component::Colony);
    for (std::vector<Component>::const_iterator it = comps.begin(); it != comps.end(); ++it)
    {
        maximum += Technology::instance().colonyModules()[(*it).ID()].population();
    }
    return maximum;
}

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

#include "Component.h"

using namespace Game;

std::string Component::typeToString(Type type)
{
    std::string name;
    switch (type)
    {
    case Game::Component::StarDrive:
        name = "Star Drive";
        break;
    case Game::Component::Colony:
        name = "Colony";
        break;
    case Game::Component::Engine:
        name = "Engine";
        break;
    default:
        name = "";
    }
    return name;
}

Component::Type Component::stringToType(const std::string & string)
{
    Type type;
    if (string == "Star Drive")
    {
        type = Game::Component::StarDrive;
    }
    else if (string == "Colony")
    {
        type = Game::Component::Colony;
    }
    else if (string == "Engine")
    {
        type = Game::Component::Engine;
    }
    else
    {
        type = Game::Component::None;
    }
    return type;
}

Component::~Component()
{
    
}

Component::Component()
    : mName()
    , mType(Component::None)
    , mLevel(0)
    , mHitPoints(0)
    , mDamage(0)
    , mCost(0)
{

}

Component::Component(const std::string & name, Component::Type type, unsigned int level, unsigned int hitPoints, int cost)
    : mName(name)
    , mType(type)
    , mLevel(level)
    , mHitPoints(hitPoints)
    , mDamage(0)
    , mCost(cost)
{

}

Component::Component(const Component & other)
    : mName(other.mName)
    , mType(other.mType)
    , mLevel(other.mLevel)
    , mHitPoints(other.mHitPoints)
    , mDamage(other.mDamage)
    , mCost(other.mCost)
{

}

Component & Component::operator =(const Component & other)
{
    if (&other != this)
    {
        mName = other.mName;
        mType = other.mType;
        mLevel = other.mLevel;
        mHitPoints = other.mHitPoints;
        mDamage = other.mDamage;
        mCost = other.mCost;
    }
    return *this;
}

const std::string & Component::name() const
{
    return mName;
}

void Component::setName(const std::string & name)
{
    mName = name;
}

Component::Type Component::type() const
{
    return mType;
}

int Component::typeIndex() const
{
    return mType;
}

void Component::setType(Component::Type type)
{
    mType = type;
}

void Component::setTypeIndex(int type)
{
    mType = Component::Type(type);
}

unsigned int Component::level() const
{
    return mLevel;
}

void Component::setLevel(unsigned int level)
{
    mLevel = level;
}

unsigned int Component::hitPoints() const
{
    return mHitPoints;
}

void Component::setHitPoints(unsigned int hitPoints)
{
    mHitPoints = hitPoints;
}

unsigned int Component::damage() const
{
    return mDamage;
}

void Component::setDamage(unsigned int damage)
{
    mDamage = damage;
}

unsigned int Component::remain() const
{
    return mHitPoints - mDamage;
}

void Component::setRemain(unsigned int remain)
{
    if (remain > mHitPoints)
    {
        remain = mHitPoints;
    }

    mDamage = mHitPoints - remain;
}

bool Component::destroyed() const
{
    return mDamage == mHitPoints;
}

int Component::cost() const
{
    return mCost;
}

void Component::setCost(int cost)
{
    mCost = cost;
}
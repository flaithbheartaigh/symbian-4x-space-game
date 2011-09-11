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

#include "Planet.h"
#include "Shipyard.h"
#include "Player.h"
#include "StarSystem.h"
#include "Universe.h"
#include "UniverseVisitor.h"

using namespace Game;

Planet::~Planet()
{
    delete mShipyard;
}

Planet::Planet()
    : mSector(NULL) 
    , mName("Unknown")
    , mPopulation(0.0f)
    , mShipyard(NULL)
    , mPlayerName()
    , mPlayer(NULL)
    , mAtmosphere(None)
    , mSize(Large)
{

}

Planet::Planet(Sector * sector)
    : mSector(sector) 
    , mName("Unknown")
    , mPopulation(0.0f)
    , mShipyard(NULL)
    , mPlayerName()
    , mPlayer(NULL)
    , mAtmosphere(None)
    , mSize(Large)
{

}

Sector * Planet::sector() const
{
    return mSector;
}

void Planet::setSector(Sector * sector)
{
    mSector = sector;
}

const std::string & Planet::name() const
{
    return mName;
}

void Planet::setName(const std::string & name)
{
    mName = name;
}

Player * Planet::player() const
{
    if (mPlayer == NULL && !mPlayerName.empty() && mSector != NULL && mSector->starSystem() != NULL && mSector->starSystem()->universe() != NULL)
    {
        return mSector->starSystem()->universe()->game().player(mPlayerName);
    }
    return mPlayer;
}

void Planet::setPlayer(Player * player)
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

std::string Planet::playerName() const
{
    if (mPlayer != NULL)
    {
        return mPlayer->name();
    }
    return mPlayerName;
}

void Planet::setPlayerName(const std::string & playerName)
{
    mPlayerName = playerName;
    if (mSector != NULL && mSector->starSystem() != NULL && mSector->starSystem()->universe() != NULL)
    {
        mPlayer = mSector->starSystem()->universe()->game().player(mPlayerName);
    }
}

float Planet::population() const
{
    return mPopulation;
}

void Planet::setPopulation(float population)
{
    mPopulation = population;
}

Planet::Atmosphere Planet::atmosphere() const
{
    return mAtmosphere;
}

void Planet::setAtmosphere(Atmosphere atmosphere)
{
    mAtmosphere = atmosphere;
}

int Planet::atmosphereIndex() const
{
    return mAtmosphere;
}

void Planet::setAtmosphereIndex(int atmosphere)
{
    mAtmosphere = Atmosphere(atmosphere);
}

Planet::Size Planet::size() const
{
    return mSize;
}

void Planet::setSize(Size size)
{
    mSize = size;
}

int Planet::sizeIndex() const
{
    return mSize;
}

void Planet::setSizeIndex(int size)
{
    mSize = Size(size);
}

Shipyard * Planet::shipyard() const
{
    return mShipyard;
}

void Planet::setShipyard(Shipyard * shipyard)
{
    mShipyard = shipyard;
}

void Planet::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
        if (mShipyard != NULL)
        {
            mShipyard->accept(visitor);
        }
    }
}
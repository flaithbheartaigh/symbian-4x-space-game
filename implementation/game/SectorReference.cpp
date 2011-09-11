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

#include "SectorReference.h"
#include "Universe.h"
#include "StarSystem.h"
#include "Sector.h"
#include "StarSystemReference.h"

using namespace Game;

SectorReference::~SectorReference()
{

}

SectorReference::SectorReference()
    : mUniverse(&Game::Universe::instance())
    , mName()
    , mX(0)
    , mY(0)
    , mCached(NULL)
{

}

SectorReference::SectorReference(Sector * sector)
    : mUniverse(NULL)
    , mName()
    , mX(0)
    , mY(0)
    , mCached(sector)
{
    if (sector != NULL && sector->starSystem() != NULL)
    {
        mUniverse = sector->starSystem()->universe();
        mName = sector->starSystem()->name();
        mX = sector->x();
        mY = sector->y();
    }
}

SectorReference::SectorReference(StarSystem * starSystem, int x, int y)
    : mUniverse(NULL)
    , mName()
    , mX(x)
    , mY(y)
    , mCached(NULL)
{
    if (starSystem != NULL)
    {
        mUniverse = starSystem->universe();
        mName = starSystem->name();
    }
}

SectorReference::SectorReference(const SectorReference & other)
    : mUniverse(other.mUniverse)
    , mName(other.mName)
    , mX(other.mX)
    , mY(other.mY)
    , mCached(other.mCached)
{

}

SectorReference & SectorReference::operator =(const SectorReference & other)
{
    if (&other != this)
    {
        mUniverse = other.mUniverse;
        mName = other.mName;
        mX = other.mX;
        mY = other.mY;
        mCached = other.mCached;
    }
    return *this;
}

bool SectorReference::operator ==(const SectorReference& other) const
{
    return mUniverse == other.mUniverse && mName == other.mName && mX == other.mX && mY == other.mY;
}

bool SectorReference::isValid() const
{
    return sector() != NULL;
}

Sector * SectorReference::sector() const
{
    if (mCached == NULL)
    {
        StarSystem * starSystem = StarSystemReference(mUniverse, mName).starSystem();
        if (starSystem != NULL)
        {
            for (std::vector<Sector *>::const_iterator it = starSystem->sectors().begin(); it != starSystem->sectors().end(); ++it)
            {
                if ((*it)->x() == mX && (*it)->y() == mY)
                {
                    mCached = *it;
                    break;
                }
            }
        }
    }
    return mCached;
}

const std::string & SectorReference::name() const
{
    return mName;
}

void SectorReference::setName(const std::string & name)
{
    mName = name;
}

int SectorReference::x() const
{
    return mX;
}

void SectorReference::setX(int x)
{
    mX = x;
}

int SectorReference::y() const
{
    return mY;
}

void SectorReference::setY(int y)
{
    mY = y;
}
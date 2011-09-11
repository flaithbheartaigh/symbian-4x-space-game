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

#include "StarSystemReference.h"
#include "Universe.h"
#include "StarSystem.h"

using namespace Game;

StarSystemReference::~StarSystemReference()
{

}

StarSystemReference::StarSystemReference()
    : mName()
    , mCached(NULL)
    , mUniverse(&Universe::instance())
{

}

StarSystemReference::StarSystemReference(StarSystem * starSystem)
    : mName()
    , mCached(starSystem)
    , mUniverse(starSystem->universe())
{
    if (starSystem != NULL)
    {
        mName = starSystem->name();
    }
}

StarSystemReference::StarSystemReference(Universe * universe, const std::string & name)
    : mName(name)
    , mCached(NULL)
    , mUniverse(universe)
{

}

StarSystemReference::StarSystemReference(Universe * universe, int x, int y)
    : mName()
    , mCached(NULL)
    , mUniverse(universe)
{
    for (std::vector<StarSystem *>::const_iterator it = mUniverse->starSystems().begin(); it != mUniverse->starSystems().end(); ++it)
    {
        if ((*it)->x() == x && (*it)->y() == y)
        {
            mCached = *it;
            break;
        }
    }
    if (mCached != NULL)
    {
        mName = mCached->name();
    }
}

StarSystemReference::StarSystemReference(const StarSystemReference & other)
    : mName(other.mName)
    , mCached(other.mCached)
    , mUniverse(other.mUniverse)
{

}

StarSystemReference & StarSystemReference::operator =(const StarSystemReference & other)
{
    if (&other != this)
    {
        mName = other.mName;
        mCached = other.mCached;
        mUniverse = other.mUniverse;
    }
    return *this;
}

bool StarSystemReference::operator ==(const StarSystemReference& other) const
{
    return mName == other.mName;
}

bool StarSystemReference::isValid() const
{
    return starSystem() != NULL;
}

StarSystem * StarSystemReference::starSystem() const
{
    if (mCached == NULL && mUniverse != NULL)
    {
        for (std::vector<StarSystem *>::const_iterator it = mUniverse->starSystems().begin(); it != mUniverse->starSystems().end(); ++it)
        {
            if ((*it)->name() == mName)
            {
                mCached = *it;
                break;
            }
        }
    }
    return mCached;
}

const std::string & StarSystemReference::name() const
{
    return mName;
}

void StarSystemReference::setName(const std::string & name)
{
    mName = name;
}
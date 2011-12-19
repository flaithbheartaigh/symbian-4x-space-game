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

#include "Warp.h"
#include "UniverseVisitor.h"

using namespace Game;

Warp::~Warp()
{
    
}

Warp::Warp()
    : mSector(NULL)
    , mDestination()
{

}

Warp::Warp(Sector * sector)
    : mSector(sector)
    , mDestination()
{

}

Sector * Warp::sector() const
{
    return mSector;
}

void Warp::setSector(Sector * sector)
{
    mSector = sector;
}

const SectorReference & Warp::destination() const
{
    return mDestination;
}

void Warp::setDestination(const SectorReference & destination)
{
    mDestination = destination;
}

void Warp::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}
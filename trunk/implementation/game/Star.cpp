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

#include "Star.h"
#include "UniverseVisitor.h"

using namespace Game;

Star::~Star()
{
    
}

Star::Star()
    : mSector(NULL)
    , mName("Unknown")
{

}

Star::Star(Sector * sector)
    : mSector(sector)
    , mName("Unknown")
{

}

Sector * Star::sector() const
{
    return mSector;
}

void Star::setSector(Sector * sector)
{
    mSector = sector;
}

const std::string & Star::name() const
{
    return mName;
}

void Star::setName(const std::string & name)
{
    mName = name;
}

void Star::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}
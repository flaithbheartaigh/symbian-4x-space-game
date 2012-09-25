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

#include "Elements.h"

using namespace Game;

Elements::~Elements()
{
    
}

Elements::Elements()
    : Hydrogen(0)
    , Uranium(0)
    , AntiHydrogen(0)
{

}

Elements::Elements(const Elements & other)
    : Hydrogen(other.Hydrogen)
    , Uranium(other.Uranium)
    , AntiHydrogen(other.AntiHydrogen)
{

}

Elements & Elements::operator =(const Elements & other)
{
    if (&other != this)
    {
        Hydrogen = other.Hydrogen;
        Uranium = other.Uranium;
        AntiHydrogen = other.AntiHydrogen;
    }
    return *this;
}

bool Elements::operator ==(const Elements & other) const
{
    if (&other == this)
    {
        return true;
    }
    if (Hydrogen != other.Hydrogen || Uranium != other.Uranium || AntiHydrogen != other.AntiHydrogen)
    {
        return false;
    }
    return true;
}
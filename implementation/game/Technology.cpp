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
{

}
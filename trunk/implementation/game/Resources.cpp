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

#include "Resources.h"

#include <sstream>

using namespace Game;

Resources * Resources::_instance = NULL;

Resources & Resources::instance()
{
    if (_instance == NULL)
    {
        _instance = new Resources(); 
    }
    return *_instance;
}

Resources::~Resources()
{
    _instance = NULL;
}

void Resources::setDataFilesPath(const std::string & dataFilesPath)
{
    mDataFilesPath = dataFilesPath;
}

const std::string & Resources::getDataFilesPath() const
{
    return mDataFilesPath;
}

std::string Resources::getDataFilePath(const std::string & path) const
{
    std::stringstream ss;
    ss << mDataFilesPath << "/" << path;
    return ss.str();
}

Resources::Resources()
    : mDataFilesPath("../data")
{

}
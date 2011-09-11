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

#include "Events.h"

using namespace Game;

Events::Listener::~Listener()
{
    instance().removeListener(this);
}

Events::Listener::Listener()
{
    instance().addListener(this);
}

Events * Events::_instance = NULL;

Events & Events::instance()
{
    if (_instance == NULL)
    {
        _instance = new Events(); 
    }
    return *_instance;
}

Events::~Events()
{
    _instance = NULL;
}

Events::Events()
    : mListeners()
    , mNextUniqueEvent(0)
{

}

void Events::addListener(Listener * listener)
{
    if (listener != NULL)
    {
        mListeners.insert(listener);
    }
}

void Events::removeListener(Listener * listener)
{
    if (listener != NULL)
    {
        mListeners.erase(listener);
    }
}

void Events::add(const Event & event)
{
    std::set<Listener *> listeners = mListeners;
    for (std::set<Listener *>::iterator it = listeners.begin(); it != listeners.end(); ++it)
    {
        (*it)->listenEvent(event);
    }
}

Event Events::next()
{
    return mNextUniqueEvent++;
}
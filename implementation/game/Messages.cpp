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

#include "Messages.h"

using namespace Game;

Messages::Subscriber::~Subscriber()
{
    instance().removeSubscriber(this);
}

Messages::Subscriber::Subscriber()
{
    instance().addSubscriber(this);
}

Messages * Messages::_instance = NULL;

Messages & Messages::instance()
{
    if (_instance == NULL)
    {
        _instance = new Messages(); 
    }
    return *_instance;
}

Messages::~Messages()
{
    _instance = NULL;
}

Messages::Messages()
    : mSubscribers()
    , mMessages()
{

}

void Messages::addSubscriber(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void Messages::removeSubscriber(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

void Messages::post(const std::string & message)
{
    mMessages.push_front(message);
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->messagePosted(message);
    }
}
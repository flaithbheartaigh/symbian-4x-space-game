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

#include "StarSystem.h"
#include "Universe.h"
#include "Sector.h"
#include "Star.h"
#include "Planet.h"
#include "Warp.h"
#include "Ship.h"
#include "UniverseVisitor.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace Game;

StarSystem::Subscriber::~Subscriber()
{
    unsubscribe();
}

StarSystem::Subscriber::Subscriber(StarSystem * starSystem)
    : mStarSystem(starSystem)
{
    if (mStarSystem != NULL)
    {
        mStarSystem->subscribe(this);
    }
}

void StarSystem::Subscriber::sectorAdded(Sector * sector)
{

}

void StarSystem::Subscriber::sectorRemoved(Sector * sector)
{

}

void StarSystem::Subscriber::becameKnown()
{

}

StarSystem * StarSystem::Subscriber::starSystem() const
{
    return mStarSystem;
}

void StarSystem::Subscriber::notify()
{
    if (mStarSystem != NULL)
    {
        if (mStarSystem->universe() != NULL && mStarSystem->universe() != NULL && !mStarSystem->universe()->notificationsBlocked())
        {
            std::vector<Sector *> sectors = mStarSystem->sectors();
            for (std::vector<Sector *>::const_iterator it = sectors.begin(); it != sectors.end(); ++it)
            {
                sectorAdded(*it);
            }
        }
    }
}

void StarSystem::Subscriber::unsubscribe()
{
    if (mStarSystem != NULL)
    {
        mStarSystem->unsubscribe(this);
        mStarSystem = NULL;
    }
}

const int StarSystem::Size = 13;

StarSystem::~StarSystem()
{
    std::vector<Sector *> sectors = mSectors;
    for (std::vector<Sector *>::const_iterator it = sectors.begin(); it != sectors.end(); ++it)
    {
        removeSector(*it);
        delete *it;
    }
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
}

StarSystem::StarSystem()
    : mUniverse(NULL)
    , mName("Unknown")
    , mX(0)
    , mY(0)
    , mSectors()
    , mSubscribers()
{

}

StarSystem::StarSystem(Universe * universe)
    : mUniverse(universe)
    , mName("Unknown")
    , mX(0)
    , mY(0)
    , mSectors()
    , mSubscribers()
{

}

Universe * StarSystem::universe() const
{
    return mUniverse;
}

void StarSystem::setUniverse(Universe * universe)
{
    mUniverse = universe;
}

void StarSystem::generate()
{
    static const int PROBABILITYPLANET = int(1000.0f * 8.0f / (Size * Size));
    static const int PROBABILITYSTAR = int(1000.0f * 0.3f / (Size * Size));
    static const int PROBABILITYWARP = int(1000.0f * 1.0f / (Size * Size));
    static const int PROBABILITYHYDROGEN = int(1000.0f * 5.0f / (Size * Size));
    static const int PROBABILITYURANIUM = int(1000.0f * 8.0f / (Size * Size));
    static const int PROBABILITYANTIHYDROGEN = int(1000.0f * 0.5f / (Size * Size));

    int planetId = 1;
    int starId = 2;

    std::vector<Sector *> generatedSectors;
    Sector * centerSector = NULL;
    for (int y = -Size / 2; y <= Size / 2; ++y)
    {
        for (int x = -Size / 2; x <= Size / 2; ++x)
        {
            Sector * sector = new Sector(this);
            sector->setX(x);
            sector->setY(y);
            generatedSectors.push_back(sector);
            if (x == 0 && y == 0)
            {
                centerSector = sector;
            }
        }
    }
    for (std::vector<Sector *>::const_iterator it = generatedSectors.begin(); it != generatedSectors.end(); ++it)
    {
        if ((*it)->x() != 0 || (*it)->y() != 0)
        {
            if (rand() % 1000 < PROBABILITYPLANET)
            {
                Game::Planet * planet = new Game::Planet(*it);
                std::ostringstream ss;
                ss << planetId++;
                planet->setName(ss.str());
                planet->setAtmosphere(Planet::Atmosphere(rand() % 5));
                planet->setSize(Planet::Size(rand() % 3));
                (*it)->setPlanet(planet);
            }
            else if (rand() % 1000 < PROBABILITYSTAR)
            {
                Game::Star * star = new Game::Star(*it);
                std::ostringstream ss;
                ss << starId++;
                star->setName(ss.str());
                (*it)->setStar(star);
            }
            else if (rand() % 1000 < PROBABILITYWARP)
            {
                Game::Warp * warp = new Game::Warp(*it);
                (*it)->setWarp(warp);
            }
            if ((*it)->planet())
            {
                if (rand() % 1000 < PROBABILITYURANIUM)
                {
                    (*it)->elements().Uranium = static_cast<unsigned int>(rand() % 100);
                }
            }
            else if (!(*it)->warp() && !(*it)->star())
            {
                if (rand() % 1000 < PROBABILITYHYDROGEN)
                {
                    (*it)->elements().Hydrogen = static_cast<unsigned int>(rand() % 1000);
                }
                if (rand() % 1000 < PROBABILITYANTIHYDROGEN)
                {
                    (*it)->elements().AntiHydrogen = static_cast<unsigned int>(rand() % 10);
                }
            }
        }
    }
    if (centerSector->star() == NULL)
    {
        Game::Star * star = new Game::Star(centerSector);
        star->setName(mName);
        centerSector->setStar(star);
    }
    setSectors(generatedSectors);
}

const std::string & StarSystem::name() const
{
    return mName;
}

void StarSystem::setName(const std::string & name)
{
    mName = name;
}

int StarSystem::x() const
{
    return mX;
}

void StarSystem::setX(int x)
{
    mX = x;
}

int StarSystem::y() const
{
    return mY;
}

void StarSystem::setY(int y)
{
    mY = y;
}

void StarSystem::addSector(Sector * sector)
{
    if (sector != NULL)
    {
        sector->setStarSystem(this);
        mSectors.push_back(sector);
        if (mUniverse != NULL && !mUniverse->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->sectorAdded(sector);
            }
        }
    }
}

void StarSystem::removeSector(Sector * sector)
{
    if (sector != NULL)
    {
        sector->setStarSystem(NULL);
        mSectors.erase(std::remove(mSectors.begin(), mSectors.end(), sector), mSectors.end());
        if (mUniverse != NULL && !mUniverse->notificationsBlocked())
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->sectorRemoved(sector);
            }
        }
    }
}

const std::vector<Sector *> & StarSystem::sectors() const
{
    return mSectors;
}

void StarSystem::setSectors(const std::vector<Sector *> & sectors)
{
    for (std::vector<Sector *>::const_iterator it = sectors.begin(); it != sectors.end(); ++it)
    {
        addSector(*it);
    }
}

std::set<Player *> StarSystem::players() const
{
    std::set<Player *> players;
    for (std::vector<Sector *>::const_iterator it = mSectors.begin(); it != mSectors.end(); ++it)
    {
        Sector * sector = *it;
        std::set<Player *> sectorPlayers = sector->players();
        players.insert(sectorPlayers.begin(), sectorPlayers.end());
    }
    return players;
}

std::set<Warp *> StarSystem::warps() const
{
    std::set<Warp *> warps;
    for (std::vector<Sector *>::const_iterator it = mSectors.begin(); it != mSectors.end(); ++it)
    {
        Sector * sector = *it;
        if (sector->warp() != NULL)
        {
            warps.insert(sector->warp());
        }
    }
    return warps;
}

float StarSystem::distance(StarSystem * starSystem) const
{
    float distance = 0;
    if (starSystem != NULL)
    {
        float diffX = float(starSystem->x() - mX);
        float diffy = float(starSystem->y() - mY);
        distance = sqrt(diffX * diffX + diffy * diffy);
    }
    return distance;
}

void StarSystem::notifyBecameKnown()
{
    if (mUniverse != NULL && !mUniverse->notificationsBlocked())
    {
        std::set<Subscriber *> subscribers = mSubscribers;
        for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->becameKnown();
        }
    }
}

void StarSystem::subscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void StarSystem::unsubscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

void StarSystem::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
        std::vector<Sector *> sectors = mSectors;
        for (std::vector<Sector *>::const_iterator it = sectors.begin(); it != sectors.end(); ++it)
        {
            if (*it != NULL)
            {
                (*it)->accept(visitor);
            }
        }
    }
}
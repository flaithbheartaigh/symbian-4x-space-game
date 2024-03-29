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

#include "Universe.h"
#include "StarSystem.h"
#include "Sector.h"
#include "Planet.h"
#include "Warp.h"
#include "Player.h"
#include "UniverseVisitor.h"
#include "NextTurnVisitor.h"
#include "NextPlayerVisitor.h"
#include "StatsVisitor.h"
#include "StarSystemReference.h"
#include "Messages.h"
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#ifdef Q_OS_BLACKBERRY
# include <time.h>
#endif
#include <ctime>
#ifdef Q_OS_BLACKBERRY
# include <stdlib.h>
#endif

using namespace Game;

Universe::Names::~Names()
{

}

Universe::Names::Names()
    : mAvailableStarNames()
    , mAvailablePlayerNames()
{

}

const std::vector<std::string> & Universe::Names::availableStarNames() const
{
    return mAvailableStarNames;
}

void Universe::Names::setAvailableStarNames(const std::vector<std::string> & starNames)
{
    mAvailableStarNames = starNames;
}

std::string Universe::Names::nextAvailableStarName()
{
    std::string nextName;
    if (mAvailableStarNames.size() > 0)
    {
        nextName = mAvailableStarNames[rand() % mAvailableStarNames.size()];
        mAvailableStarNames.erase(std::remove(mAvailableStarNames.begin(), mAvailableStarNames.end(), nextName), mAvailableStarNames.end());  
    }
    else
    {
        static int i = 0;
        std::ostringstream ss;
        ss << "Unnamed system " << i;
        ++i;
        nextName = ss.str();
    }
    return nextName;
}

const std::vector<std::string> & Universe::Names::availablePlayerNames() const
{
    return mAvailablePlayerNames;
}

void Universe::Names::setAvailablePlayerNames(const std::vector<std::string> & playerNames)
{
    mAvailablePlayerNames = playerNames;
}

std::string Universe::Names::nextAvailablePlayerName()
{
    std::string nextName;
    if (mAvailablePlayerNames.size() > 0)
    {
        nextName = mAvailablePlayerNames[rand() % mAvailablePlayerNames.size()];
        mAvailablePlayerNames.erase(std::remove(mAvailablePlayerNames.begin(), mAvailablePlayerNames.end(), nextName), mAvailablePlayerNames.end());  
    }
    else
    {
        static int i = 0;
        std::ostringstream ss;
        ss << "Unnamed player" << i;
        ++i;
        nextName = ss.str();
    }
    return nextName;
}

void Universe::Names::clear()
{
    mAvailableStarNames.clear();
    mAvailablePlayerNames.clear();
}

Universe::Game::Subscriber::~Subscriber()
{
    unsubscribe();
}

Universe::Game::Subscriber::Subscriber()
    : mUniverse(&Universe::instance())
{
    if (mUniverse != NULL)
    {
        mUniverse->game().subscribe(this);
    }
}

Universe::Game::Subscriber::Subscriber(Universe * universe)
    : mUniverse(universe)
{
    if (mUniverse != NULL)
    {
        mUniverse->game().subscribe(this);
    }
}

void Universe::Game::Subscriber::playerDeactivated(Player *)
{

}

void Universe::Game::Subscriber::playerActivated(Player *)
{

}

Universe * Universe::Game::Subscriber::universe() const
{
    return mUniverse;
}

void Universe::Game::Subscriber::notify()
{
    if (mUniverse != NULL)
    {
        if (!mUniverse->notificationsBlocked())
        {
            playerActivated(mUniverse->game().currentPlayer());
        }
    }
}

void Universe::Game::Subscriber::unsubscribe()
{
    if (mUniverse != NULL)
    {
        mUniverse->game().unsubscribe(this);
        mUniverse = NULL;
    }
}

Universe::Game::~Game()
{
    clear();
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
}

Universe::Game::Game()
    : mUniverse(NULL)
    , mPlayers()
    , mCurrentPlayerIndex(0)
    , mSubscribers()
{

}

void Universe::Game::setUniverse(Universe * universe)
{
    mUniverse = universe;
}

const std::vector<Player *> & Universe::Game::players() const
{
    return mPlayers;
}

void Universe::Game::setPlayers(const std::vector<Player *> & players)
{
    mPlayers = players;
}

void Universe::Game::addPlayer(Player * player)
{
    if (player != NULL)
    {
        for (std::vector<Player *>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
        {
            if ((*it)->name() == player->name())
            {
                throw "void Universe::Game::addPlayer(Player *): Player already exists.\n";
            }
        }
        mPlayers.push_back(player);
    }
    if (mPlayers.size() == 1)
    {
        setCurrentPlayerIndex(0);
    }
}

Player * Universe::Game::currentPlayer() const
{
    std::vector<Player *> players = mPlayers;
    if (players.size() > 0)
    {
        return players[mCurrentPlayerIndex];
    }
    return NULL;
}

Player * Universe::Game::player(const std::string & name) const
{
    if (!name.empty())
    {
        for (std::vector<Player *>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
        {
            if ((*it)->name() == name)
            {
                return *it;
            }
        }
    }
    return NULL;
}

unsigned int Universe::Game::playerIndex(Player * player) const
{
    if (player != NULL)
    {
        unsigned int i = 0;
        for (std::vector<Player *>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
        {
            if (*it == player)
            {
                return i;
            }
            ++i;
        }
    }
    throw "void Universe::Game::playerIndex(Player *): Player doesn't exists.\n";
}

void Universe::Game::setCurrentPlayerIndex(unsigned int index)
{
    Player * previousPlayer = currentPlayer();
    mCurrentPlayerIndex = index;
    if (mUniverse != NULL && !mUniverse->notificationsBlocked())
    {
        std::set<Subscriber *> subscribers = mSubscribers;
        for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->playerDeactivated(previousPlayer);
            (*it)->playerActivated(currentPlayer());
        }
    }
}

unsigned int Universe::Game::currentPlayerIndex() const
{
    return mCurrentPlayerIndex;
}

bool Universe::Game::haveAllPlayersPlayed() const
{
    return mPlayers.size() <= 0 || mCurrentPlayerIndex >= mPlayers.size() - 1;
}

void Universe::Game::subscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void Universe::Game::unsubscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

void Universe::Game::clear()
{
    for (std::vector<Player *>::const_iterator it = mPlayers.begin(); it != mPlayers.end(); ++it)
    {
        delete *it;
    }
        mPlayers.clear();

    mCurrentPlayerIndex = 0;
}

Universe::Subscriber::~Subscriber()
{
    unsubscribe();
}

Universe::Subscriber::Subscriber()
    : mUniverse(&Universe::instance())
{
    if (mUniverse != NULL)
    {
        mUniverse->subscribe(this);
    }
}

Universe::Subscriber::Subscriber(Universe * universe)
    : mUniverse(universe)
{
    if (mUniverse != NULL)
    {
        mUniverse->subscribe(this);
    }
}

void Universe::Subscriber::starSystemAdded(StarSystem * starSystem)
{

}

void Universe::Subscriber::starSystemRemoved(StarSystem * starSystem)
{

}

Universe * Universe::Subscriber::universe() const
{
    return mUniverse;
}

void Universe::Subscriber::notify()
{
    if (mUniverse != NULL)
    {
        if (!mUniverse->notificationsBlocked())
        {
            std::vector<StarSystem *> starSystems = mUniverse->starSystems();
            for (std::vector<StarSystem *>::const_iterator it = starSystems.begin(); it != starSystems.end(); ++it)
            {
                starSystemAdded(*it);
            }
        }
    }
}

void Universe::Subscriber::unsubscribe()
{
    if (mUniverse != NULL)
    {
        mUniverse->unsubscribe(this);
        mUniverse = NULL;
    }
}

Universe * Universe::_instance = NULL;

Universe & Universe::instance()
{
    if (_instance == NULL)
    {
        _instance = new Universe(); 
    }
    return *_instance;
}

Universe::~Universe()
{
    clear();
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
    _instance = NULL;
}

Universe::Universe()
    : mCurrentTurn(0)
    , mStarSystems()
    , mSubscribers()
    , mNames()
    , mGame()
    , mNotificationsBlocked(false)
{
    mGame.setUniverse(this);
    srand(static_cast<unsigned int>(time(NULL)));
}

void Universe::update(bool stopOnNPC, bool stopOnHuman)
{
    do
    {
        if (mGame.haveAllPlayersPlayed())
        {
            accept(&NextTurnVisitor());
        }
        accept(&NextPlayerVisitor());

        std::ostringstream ss;
        ss << "Year " << (2200 + mCurrentTurn / 10.0);
        if (game().currentPlayer() != NULL)
        {
            ss << ", " << game().currentPlayer()->name() << ", " << game().currentPlayer()->money() << "C$ (" << game().currentPlayer()->revenue() << ")";
        }
        Messages::instance().post(ss.str());
    }
    while (game().currentPlayer() != NULL && !(stopOnHuman && game().currentPlayer()->isHuman()) && !(stopOnNPC && !game().currentPlayer()->isHuman()));
}

void Universe::setCurrentTurn(unsigned int turn)
{
    mCurrentTurn = turn;
}

unsigned int Universe::currentTurn() const
{
    return mCurrentTurn;
}

const std::vector<StarSystem *> & Universe::starSystems() const
{
    return mStarSystems;
}

void Universe::setStarSystems(const std::vector<StarSystem *> & starSystems)
{
    for (std::vector<StarSystem *>::const_iterator it = starSystems.begin(); it != starSystems.end(); ++it)
    {
        addStarSystem(*it);
    }
}

void Universe::addStarSystem(StarSystem * starSystem)
{
    if (starSystem != NULL)
    {
        starSystem->setUniverse(this);
        mStarSystems.push_back(starSystem);
        if (!mNotificationsBlocked)
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->starSystemAdded(starSystem);
            }
        }
    }
}

void Universe::removeStarSystem(StarSystem * starSystem)
{
    if (starSystem != NULL)
    {
        starSystem->setUniverse(NULL);
        mStarSystems.erase(std::remove(mStarSystems.begin(), mStarSystems.end(), starSystem), mStarSystems.end());
        if (!mNotificationsBlocked)
        {
            std::set<Subscriber *> subscribers = mSubscribers;
            for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
            {
                (*it)->starSystemRemoved(starSystem);
            }
        }
    }
}

void Universe::subscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void Universe::unsubscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

void Universe::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
        std::vector<Player *> players = mGame.players();
        for (std::vector<Player *>::const_iterator it = players.begin(); it != players.end(); ++it)
        {
            if (*it != NULL)
            {
                (*it)->accept(visitor);
            }
        }
        std::vector<StarSystem *> starSystems = mStarSystems;
        for (std::vector<StarSystem *>::const_iterator it = starSystems.begin(); it != starSystems.end(); ++it)
        {
            if (*it != NULL)
            {
                (*it)->accept(visitor);
            }
        }
    }
}

const Universe::Names & Universe::names() const
{
    return mNames;
}

const Universe::Game & Universe::game() const
{
    return mGame;
}

Universe::Names & Universe::names()
{
    return mNames;
}

Universe::Game & Universe::game()
{
    return mGame;
}

void Universe::generate(unsigned int density, unsigned int size, unsigned int connections)
{
    const int UNIVERSESIZE = size;
    const int UNIVERSECOUNT = density;
    for (int x = -UNIVERSESIZE; x < UNIVERSESIZE; ++x)
    {
        for (int y = -UNIVERSESIZE; y < UNIVERSESIZE; ++y)
        {
            if (rand() % UNIVERSESIZE < UNIVERSECOUNT)
            {
                StarSystem * starSystem = new StarSystem(this);
                starSystem->setX(x);
                starSystem->setY(y);
                starSystem->setName(mNames.nextAvailableStarName());
                addStarSystem(starSystem);
                starSystem->generate(connections);
            }
        }
    }

    std::vector<Warp *> warps;

    class PrivateWarpVisitor
        : public UniverseVisitor
    {

    public:

        PrivateWarpVisitor(std::vector<Warp *> * warps)
            : UniverseVisitor()
            , mWarps(warps)
        {

        }

    private:

        void visit(Warp * warp)
        {
            if (mWarps != NULL)
            {
                mWarps->push_back(warp);
            }
        }

        std::vector<Warp *> * mWarps;

    } visitor(&warps);
    accept(&visitor);

    while (warps.size() > 1)
    {
        Warp * a = warps[rand() % warps.size()];
        Warp * b = warps[rand() % warps.size()];
        warps.erase(std::remove(warps.begin(), warps.end(), a), warps.end());
        if (a != b)
        {
            a->setDestination(SectorReference(b->sector()));
            b->setDestination(SectorReference(a->sector()));
            warps.erase(std::remove(warps.begin(), warps.end(), b), warps.end());
            a->sector()->notifyChanged(Sector::HasWarp);
            b->sector()->notifyChanged(Sector::HasWarp);
        }
    }

    if (connections > 0)
    {
        for (unsigned int i = 0; i < mStarSystems.size() - 1; ++i)
        {
            if (connections > 1 || rand() % 2 == 0)
            {
                bool done = false;
                int count = 10;
                while (!done && count > 0)
                {
                    --count;
                    Sector * from = mStarSystems[i]->sectors()[rand() % mStarSystems[i]->sectors().size()];
                    Sector * to = mStarSystems[i+1]->sectors()[rand() % mStarSystems[i+1]->sectors().size()];
                    done = Warp::connect(from, to);
                }
            }
        }
    }

    std::set<Planet *> homeworlds;
    StatsVisitor statsVisitor(NULL);
    accept(&statsVisitor);
    if (statsVisitor.mPlanets.size() > 0)
    {
        for (std::vector<Player *>::const_iterator it = mGame.players().begin(); it != mGame.players().end(); ++it)
        {
            Planet * planet = statsVisitor.mPlanets[rand() % statsVisitor.mPlanets.size()];
            int max = 100;
            while (homeworlds.find(planet) != homeworlds.end() && max > 0)
            {
                --max;
                planet = statsVisitor.mPlanets[rand() % statsVisitor.mPlanets.size()];
            }
            if (homeworlds.find(planet) == homeworlds.end())
            {
                homeworlds.insert(planet);
                planet->setPlayer(*it);
                planet->setPopulation(100);
                (*it)->addKnownSystem(StarSystemReference(planet->sector()->starSystem()));
                (*it)->setHomeSector(planet->sector());
                (*it)->setSelectedSector(planet->sector());
            }
        }
    }
    setCurrentTurn(0);

    Messages::instance().post("New Universe spawned");
}

bool Universe::notificationsBlocked() const
{
    return mNotificationsBlocked;
}

bool Universe::blockNotifications(bool block)
{
    bool previous = mNotificationsBlocked;
    mNotificationsBlocked = block;
    return previous;
}

void Universe::clear()
{
    for (std::vector<Player *>::const_iterator it = mGame.players().begin(); it != mGame.players().end(); ++it)
    {
        (*it)->setSelectedSector(NULL);
        //mGame.setSelectedSector(NULL, *it);
        (*it)->setHomeSector(NULL);
    }
    std::vector<StarSystem *> starSystems = mStarSystems;
    for (std::vector<StarSystem *>::const_iterator it = starSystems.begin(); it != starSystems.end(); ++it)
    {
        removeStarSystem(*it);
        delete *it;
    }
    mNames.clear();
    mGame.clear();
    mCurrentTurn = 0;
    mNotificationsBlocked = false; 
}

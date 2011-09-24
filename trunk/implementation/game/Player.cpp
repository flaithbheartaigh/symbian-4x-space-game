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

#include "Player.h"
#include "AI.h"
#include "Sector.h"
#include "StarSystem.h"
#include "UniverseVisitor.h"
#include "StatsVisitor.h"
#include "Planet.h"
#include "Ship.h"
#include "ShipConfig.h"
#include <algorithm>

using namespace Game;

Player::Current::Subscriber::~Subscriber()
{
    unsubscribe();
}

Player::Current::Subscriber::Subscriber()
{
    Player::Current::instance().subscribe(this);
}

void Player::Current::Subscriber::selectedSectorChanged(Sector * sector)
{

}

void Player::Current::Subscriber::unsubscribe()
{
    Player::Current::instance().unsubscribe(this);
}

Player::Current * Player::Current::_instance = NULL;

Player::Current & Player::Current::instance()
{
    if (_instance == NULL)
    {
        _instance = new Player::Current(); 
    }
    return *_instance;
}

Player::Current::~Current()
{
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->unsubscribe();
    }
    _instance = NULL;
}

void Player::Current::playerDeactivated(Player * player)
{
    if (player != NULL && player->selectedSector().sector() != NULL)
    {
        player->selectedSector().sector()->notifyDeselected();
    }
}

void Player::Current::playerActivated(Player * player)
{
    if (player != NULL && player->selectedSector().sector() != NULL)
    {
        player->selectedSector().sector()->notifySelected();
    }
    std::set<Subscriber *> subscribers = mSubscribers;
    for (std::set<Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->selectedSectorChanged(player != NULL ? player->selectedSector().sector() : NULL);
    }
}

Player::Current::Current()
    : Universe::Game::Subscriber()
    , mSubscribers()
    , mSelectionAllowed(true)
{

}

void Player::Current::setSelectionAllowed(bool allowed)
{
    mSelectionAllowed = allowed;
}

bool Player::Current::selectionAllowed() const
{
    return mSelectionAllowed;
}

void Player::Current::subscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.insert(subscriber);
    }
}

void Player::Current::unsubscribe(Subscriber * subscriber)
{
    if (subscriber != NULL)
    {
        mSubscribers.erase(subscriber);
    }
}

const int Player::Colors[4][3] =
{
    { 255,  95,  95 },
    {  95, 255,  95 },
    {  95,  95, 255 },
    { 255,  95, 255 }
};

std::vector<int> Player::color(int index)
{
    std::vector<int> color;
    color.resize(3);
    std::copy(Colors[index], Colors[index]+3, color.begin());
    return color;
}

Player::~Player()
{
    delete mAI;
    mAI = NULL;
}

Player::Player()
    : mName("Unknown")
    , mShipConfigs()
    , mComponents()
    , mMoney(0)
    , mSelectedSector()
    , mHomeSector()
    , mKnownSystems()
    , mAI(NULL)
{

}

void Player::update()
{

}

const std::string & Player::name() const
{
    return mName;
}

void Player::setName(const std::string & name)
{
    mName = name;
}

int Player::money() const
{
    return mMoney;
}

void Player::setMoney(int money)
{
    mMoney = money;
}

const std::vector<ShipConfig> & Player::shipConfigs() const
{
    return mShipConfigs;
}

void Player::setShipConfigs(const std::vector<ShipConfig> & shipConfigs)
{
    mShipConfigs = shipConfigs;
}

ShipConfig Player::shipConfig(const std::string & configName) const
{
    ShipConfig shipConfig;
    for (std::vector<ShipConfig>::const_iterator it = mShipConfigs.begin(); it != mShipConfigs.end(); ++it)
    {
        if ((*it).name() == configName)
        {
            shipConfig = *it;
            break;
        }
    }
    return shipConfig;
}

void Player::addShipConfig(const ShipConfig & shipConfig)
{
    bool updated = false;
    for (std::vector<ShipConfig>::iterator it = mShipConfigs.begin(); it != mShipConfigs.end(); ++it)
    {
        if ((*it).name() == shipConfig.name())
        {
            (*it) = shipConfig;
            updated = true;
        }
    }
    if (!updated)
    {
        mShipConfigs.push_back(shipConfig);
    }
}

const std::vector<Component> & Player::components() const
{
    return mComponents;
}

void Player::setComponents(const std::vector<Component> & components)
{
    mComponents = components;
}

const SectorReference & Player::selectedSector() const
{
    return mSelectedSector;
}

void Player::setSelectedSector(const SectorReference & selectedSector)
{
    if (Current::instance().selectionAllowed())
    {
        SectorReference previous = mSelectedSector;
        mSelectedSector = selectedSector;
        if (previous.sector() != NULL)
        {
            previous.sector()->notifyDeselected();
        }
        if (mSelectedSector.sector() != NULL)
        {
            mSelectedSector.sector()->notifySelected();
        }
        std::set<Current::Subscriber *> subscribers = Current::instance().mSubscribers;
        for (std::set<Current::Subscriber *>::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
        {
            (*it)->selectedSectorChanged(mSelectedSector.sector());
        }
    }
}

const SectorReference & Player::homeSector() const
{
    return mHomeSector;
}

void Player::setHomeSector(const SectorReference & homeSector)
{
    mHomeSector = homeSector;
}

const std::vector<StarSystemReference> & Player::knownSystems() const
{
    return mKnownSystems;
}

void Player::setKnownSystems(const std::vector<StarSystemReference> & knownSystems)
{
    for (std::vector<StarSystemReference>::const_iterator it = knownSystems.begin(); it != knownSystems.end(); ++it)
    {
        addKnownSystem(*it);
    }
}

void Player::addKnownSystem(const StarSystemReference & knownSystem)
{
    if (std::find(mKnownSystems.begin(), mKnownSystems.end(), knownSystem) == mKnownSystems.end())
    {
        mKnownSystems.push_back(knownSystem);
        if (knownSystem.starSystem() != NULL)
        {
            knownSystem.starSystem()->notifyBecameKnown();
        }
    }
}

bool Player::knows(StarSystem * starSystem) const
{
    return starSystem != NULL && std::find(mKnownSystems.begin(), mKnownSystems.end(), StarSystemReference(starSystem)) != mKnownSystems.end();
}

void Player::accept(UniverseVisitor * visitor)
{
    if (visitor != NULL)
    {
        visitor->visit(this);
    }
}

AI * Player::ai() const
{
    return mAI;
}

void Player::setAI(AI * ai)
{
    mAI = ai;
}

bool Player::isHuman() const
{
    return dynamic_cast<NPC *>(mAI) == NULL;
}

int Player::revenue() const
{
    int rev = 0;
    Game::StatsVisitor stats(this);
    Game::Universe::instance().accept(&stats);        
    for (unsigned int i = 0; i < stats.mPlanets.size(); ++i)
    {
        if (stats.mPlanets[i] != NULL)
        {
            rev += int(stats.mPlanets[i]->population() / 10);
        }
    }
    for (unsigned int i = 0; i < stats.mShips.size(); ++i)
    {
        if (stats.mShips[i] != NULL)
        {
            rev -= int(stats.mShips[i]->config().cost() / 10);
        }
    }

    return rev;
}
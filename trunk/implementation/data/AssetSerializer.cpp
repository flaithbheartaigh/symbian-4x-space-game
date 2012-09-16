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

#include "AssetSerializer.h"

#include <game/Component.h>
#include <game/Technology.h>
#include <game/Universe.h>

#include <game/Player.h>
#include <game/StarSystem.h>
#include <game/Sector.h>
#include <game/Star.h>
#include <game/Planet.h>
#include <game/Warp.h>
#include <game/Ship.h>

#include <vector>
#include <string>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>

namespace
{
    /// Object to Json
    ///
    template <class T>
    void serialize(T * valueFrom, Json::Value & valueTo);

    /// Json to existing object pointer
    ///
    template <class T>
    void deserialize(const Json::Value & valueFrom, T * existingObject);

    /// Json to new object reference
    ///
    template <class T, class U>
    void deserializeObject(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(U *));

    /// Json to new object reference
    ///
    template <class T, class U>
    void deserializeObject(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(const U &));

    /// Json to value
    ///
    template <class T, class U>
    void deserializeValue(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(U));

    /// Value to Json
    ///
    template <class T>
    void serialize(T valueFrom, Json::Value & valueTo);

    /// std::vector of values to Json array
    ///
    template <typename T>
    void serialize(const std::vector<T> & vector, Json::Value & parent);

    /// Json array to std::vector of values
    ///
    template <class T>
    void deserialize(const Json::Value & value, std::vector<T> & vector);

    /// std::vector of objects to Json array
    ///
    template <typename T>
    void serialize(const std::vector<T *> & vector, Json::Value & parent);

    /// Json array to std::vector of objects
    ///
    template <class T>
    void deserialize(const Json::Value & value, std::vector<T *> & vector);

    /// Json to string
    ///
    void deserialize(const Json::Value & valueFrom, std::string & valueTo)
    {
        valueTo = valueFrom.asString();
    }

    /// Json to float
    ///
    void deserialize(const Json::Value & valueFrom, float & valueTo)
    {
        valueTo = float(valueFrom.asDouble());
    }

    /// Json to bool
    ///
    void deserialize(const Json::Value & valueFrom, bool & valueTo)
    {
        valueTo = valueFrom.asBool();
    }

    /// Json to int
    ///
    void deserialize(const Json::Value & valueFrom, int & valueTo)
    {
        valueTo = valueFrom.asInt();
    }

    /// Unsigned int to Json
    ///
    void serialize(const unsigned int & valueFrom, Json::Value & valueTo)
    {
        valueTo = int(valueFrom);
    }

    /// Json to unsigned int
    ///
    void deserialize(const Json::Value & valueFrom, unsigned int & valueTo)
    {
        valueTo = static_cast<unsigned int>(valueFrom.asInt());
    }

    // Component
    void serialize(const Game::Component & component, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_TYPE = "Type";
        static const char * PROPERTY_VALUE = "Value";
        static const char * PROPERTY_HITPOINTS = "HitPoints";
        static const char * PROPERTY_DAMAGE = "Damage";
        static const char * PROPERTY_COST = "Cost";

        serialize(component.name(), value[PROPERTY_NAME]);
        serialize(component.typeIndex(), value[PROPERTY_TYPE]);
        serialize(component.value(), value[PROPERTY_VALUE]);
        serialize(component.hitPoints(), value[PROPERTY_HITPOINTS]);
        serialize(component.damage(), value[PROPERTY_DAMAGE]);
        serialize(component.cost(), value[PROPERTY_COST]);
    }

    void deserialize(const Json::Value & value, Game::Component & component)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_TYPE = "Type";
        static const char * PROPERTY_VALUE = "Value";
        static const char * PROPERTY_HITPOINTS = "HitPoints";
        static const char * PROPERTY_DAMAGE = "Damage";
        static const char * PROPERTY_COST = "Cost";

        deserializeObject(value[PROPERTY_NAME], component, &Game::Component::setName);
        deserializeValue(value[PROPERTY_TYPE], component, &Game::Component::setTypeIndex);
        deserializeValue(value[PROPERTY_VALUE], component, &Game::Component::setValue);
        deserializeValue(value[PROPERTY_HITPOINTS], component, &Game::Component::setHitPoints);
        deserializeValue(value[PROPERTY_DAMAGE], component, &Game::Component::setDamage);
        deserializeValue(value[PROPERTY_COST], component, &Game::Component::setCost);
    }

    // Technology
    void serialize(const Game::Technology & technology, Json::Value & value)
    {
        static const char * PROPERTY_COMPONENTS = "Components";

        serialize(technology.components(), value[PROPERTY_COMPONENTS]);
    }

    void deserialize(const Json::Value & value, Game::Technology & technology)
    {
        static const char * PROPERTY_COMPONENTS = "Components";

        deserializeObject(value[PROPERTY_COMPONENTS], technology, &Game::Technology::setComponents);
    }

    // Universe
    void serialize(const Game::ShipConfig & shipConfig, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_COMPONENTS = "Components";

        serialize(shipConfig.name(), value[PROPERTY_NAME]);
        serialize(shipConfig.components(), value[PROPERTY_COMPONENTS]);
    }

    void deserialize(const Json::Value & value, Game::ShipConfig & shipConfig)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_COMPONENTS = "Components";

        deserializeObject(value[PROPERTY_NAME], shipConfig, &Game::ShipConfig::setName);
        deserializeObject(value[PROPERTY_COMPONENTS], shipConfig, &Game::ShipConfig::setComponents);
    }

    void serialize(const Game::SectorReference & sectorReference, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";

        serialize(sectorReference.name(), value[PROPERTY_NAME]);
        serialize(sectorReference.x(), value[PROPERTY_X]);
        serialize(sectorReference.y(), value[PROPERTY_Y]);
    }

    void deserialize(const Json::Value & value, Game::SectorReference & sectorReference)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";

        deserializeObject(value[PROPERTY_NAME], sectorReference, &Game::SectorReference::setName);
        deserializeValue(value[PROPERTY_X], sectorReference, &Game::SectorReference::setX);
        deserializeValue(value[PROPERTY_Y], sectorReference, &Game::SectorReference::setY);
    }

    void serialize(const Game::StarSystemReference & starSystemReference, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";

        serialize(starSystemReference.name(), value[PROPERTY_NAME]);
    }

    void deserialize(const Json::Value & value, Game::StarSystemReference & starSystemReference)
    {
        static const char * PROPERTY_NAME = "Name";
 
        deserializeObject(value[PROPERTY_NAME], starSystemReference, &Game::StarSystemReference::setName);
    }

    void serialize(const Game::Ship & ship, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_PLAYER = "Player";
        static const char * PROPERTY_TRANSIT = "Transit";
        static const char * PROPERTY_POPULATION = "Population";
        static const char * PROPERTY_MOVEMENT = "Movement";
        static const char * PROPERTY_DESTINATION = "Destination";
        static const char * PROPERTY_ARRIVAL = "Arrival";
        static const char * PROPERTY_SHIPCONFIG = "ShipConfig";

        serialize(ship.name(), value[PROPERTY_NAME]);
        serialize(ship.playerName(), value[PROPERTY_PLAYER]);
        serialize(ship.isInTransit(), value[PROPERTY_TRANSIT]);
        serialize(ship.population(), value[PROPERTY_POPULATION]);
        serialize(ship.movement(), value[PROPERTY_MOVEMENT]);
        serialize(ship.destination(), value[PROPERTY_DESTINATION]);
        serialize(ship.arrival(), value[PROPERTY_ARRIVAL]);
        serialize(ship.config(), value[PROPERTY_SHIPCONFIG]);
    }

    void deserialize(const Json::Value & value, Game::Ship & ship)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_PLAYER = "Player";
        static const char * PROPERTY_TRANSIT = "Transit";
        static const char * PROPERTY_POPULATION = "Population";
        static const char * PROPERTY_MOVEMENT = "Movement";
        static const char * PROPERTY_DESTINATION = "Destination";
        static const char * PROPERTY_ARRIVAL = "Arrival";
        static const char * PROPERTY_SHIPCONFIG = "ShipConfig";

        deserializeObject(value[PROPERTY_NAME], ship, &Game::Ship::setName);
        deserializeObject(value[PROPERTY_PLAYER], ship, &Game::Ship::setPlayerName);
        deserializeValue(value[PROPERTY_TRANSIT], ship, &Game::Ship::setInTransit);
        deserializeValue(value[PROPERTY_POPULATION], ship, &Game::Ship::setPopulation);
        deserializeValue(value[PROPERTY_MOVEMENT], ship, &Game::Ship::setMovement);
        deserializeObject(value[PROPERTY_DESTINATION], ship, &Game::Ship::setDestination);
        deserializeValue(value[PROPERTY_ARRIVAL], ship, &Game::Ship::setArrival);
        deserializeObject(value[PROPERTY_SHIPCONFIG], ship, &Game::Ship::setConfig);
    }

    void serialize(const Game::Planet & planet, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_PLAYER = "Player";
        static const char * PROPERTY_POPULATION = "Population";
        static const char * PROPERTY_ATMOSPHERE = "Atmosphere";
        static const char * PROPERTY_SIZE = "Size";

        serialize(planet.name(), value[PROPERTY_NAME]);
        serialize(planet.playerName(), value[PROPERTY_PLAYER]);
        serialize(planet.population(), value[PROPERTY_POPULATION]);
        serialize(planet.atmosphereIndex(), value[PROPERTY_ATMOSPHERE]);
        serialize(planet.sizeIndex(), value[PROPERTY_SIZE]);
    }

    void deserialize(const Json::Value & value, Game::Planet & planet)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_PLAYER = "Player";
        static const char * PROPERTY_POPULATION = "Population";
        static const char * PROPERTY_ATMOSPHERE = "Atmosphere";
        static const char * PROPERTY_SIZE = "Size";

        deserializeObject(value[PROPERTY_NAME], planet, &Game::Planet::setName);
        deserializeObject(value[PROPERTY_PLAYER], planet, &Game::Planet::setPlayerName);
        deserializeValue(value[PROPERTY_POPULATION], planet, &Game::Planet::setPopulation);
        deserializeValue(value[PROPERTY_ATMOSPHERE], planet, &Game::Planet::setAtmosphereIndex);
        deserializeValue(value[PROPERTY_SIZE], planet, &Game::Planet::setSizeIndex);
    }

    void serialize(const Game::Warp & warp, Json::Value & value)
    {
        static const char * PROPERTY_DESTINATION = "Destination";

        serialize(warp.destination(), value[PROPERTY_DESTINATION]);
    }

    void deserialize(const Json::Value & value, Game::Warp & warp)
    {
        static const char * PROPERTY_DESTINATION = "Destination";

        deserializeObject(value[PROPERTY_DESTINATION], warp, &Game::Warp::setDestination);
    }

    void serialize(const Game::Star & star, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";

        serialize(star.name(), value[PROPERTY_NAME]);
    }

    void deserialize(const Json::Value & value, Game::Star & star)
    {
        static const char * PROPERTY_NAME = "Name";

        deserializeObject(value[PROPERTY_NAME], star, &Game::Star::setName);
    }

    void serialize(const Game::Sector & sector, Json::Value & value)
    {
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";
        static const char * PROPERTY_STAR = "Star";
        static const char * PROPERTY_PLANET = "Planet";
        static const char * PROPERTY_WARP = "Warp";
        static const char * PROPERTY_SHIPS = "Ships";
        static const char * PROPERTY_TRANSITSHIPS = "TransitShips";
        static const char * PROPERTY_EL1 = "El1";
        static const char * PROPERTY_EL2 = "El2";
        static const char * PROPERTY_EL3 = "El3";

        serialize(sector.x(), value[PROPERTY_X]);
        serialize(sector.y(), value[PROPERTY_Y]);
        serialize(sector.star(), value[PROPERTY_STAR]);
        serialize(sector.planet(), value[PROPERTY_PLANET]);
        serialize(sector.warp(), value[PROPERTY_WARP]);
        serialize(sector.ships(), value[PROPERTY_SHIPS]);
        serialize(sector.shipsInTransit(), value[PROPERTY_TRANSITSHIPS]);
        value[PROPERTY_EL1] = sector.elements().Hydrogen;
        value[PROPERTY_EL2] = sector.elements().Uranium;
        value[PROPERTY_EL3] = sector.elements().AntiHydrogen;
    }

    void serialize(const std::vector<Game::Sector *> & vector, Json::Value & parent)
    {
        for (std::vector<Game::Sector *>::const_iterator it = vector.begin(); it != vector.end(); ++it)
        {
            if (!(*it)->isEmpty())
            {
                Json::Value value(Json::objectValue);
                serialize(*(*it), value);
                parent.append(value);
            }
        }
    }

    void deserialize(const Json::Value & value, Game::Sector & sector)
    {
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";
        static const char * PROPERTY_STAR = "Star";
        static const char * PROPERTY_PLANET = "Planet";
        static const char * PROPERTY_WARP = "Warp";
        static const char * PROPERTY_SHIPS = "Ships";
        static const char * PROPERTY_TRANSITSHIPS = "TransitShips";
        static const char * PROPERTY_EL1 = "El1";
        static const char * PROPERTY_EL2 = "El2";
        static const char * PROPERTY_EL3 = "El3";

        deserializeValue(value[PROPERTY_X], sector, &Game::Sector::setX);
        deserializeValue(value[PROPERTY_Y], sector, &Game::Sector::setY);
        deserializeObject(value[PROPERTY_STAR], sector, &Game::Sector::setStar);
        deserializeObject(value[PROPERTY_PLANET], sector, &Game::Sector::setPlanet);
        deserializeObject(value[PROPERTY_WARP], sector, &Game::Sector::setWarp);
        deserializeObject(value[PROPERTY_SHIPS], sector, &Game::Sector::addShips);
        deserializeObject(value[PROPERTY_TRANSITSHIPS], sector, &Game::Sector::addShipsInTransit);
        sector.elements().Hydrogen = value[PROPERTY_EL1].asUInt();
        sector.elements().Uranium = value[PROPERTY_EL2].asUInt();
        sector.elements().AntiHydrogen = value[PROPERTY_EL3].asUInt();
    }

    void serialize(const Game::StarSystem & starSystem, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";
        static const char * PROPERTY_SECTORS = "Sectors";

        serialize(starSystem.name(), value[PROPERTY_NAME]);
        serialize(starSystem.x(), value[PROPERTY_X]);
        serialize(starSystem.y(), value[PROPERTY_Y]);
        serialize(starSystem.sectors(), value[PROPERTY_SECTORS]);
    }

    void deserialize(const Json::Value & value, Game::StarSystem & starSystem)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_X = "X";
        static const char * PROPERTY_Y = "Y";
        static const char * PROPERTY_SECTORS = "Sectors";

        deserializeObject(value[PROPERTY_NAME], starSystem, &Game::StarSystem::setName);
        deserializeValue(value[PROPERTY_X], starSystem, &Game::StarSystem::setX);
        deserializeValue(value[PROPERTY_Y], starSystem, &Game::StarSystem::setY);
        deserializeObject(value[PROPERTY_SECTORS], starSystem, &Game::StarSystem::setSectors);
    }

    void serialize(const Game::Universe & universe, Json::Value & value)
    {
        static const char * PROPERTY_PLAYERS = "Players";
        static const char * PROPERTY_STARSYSTEMS = "StarSystems";
        static const char * PROPERTY_AVAILABLESTARNAMES = "AvailableStarNames";
        static const char * PROPERTY_AVAILABLEPLAYERNAMES = "AvailablePlayerNames";
        static const char * PROPERTY_CURRENTPLAYERINDEX = "CurrentPlayer";
        static const char * PROPERTY_CURRENTTURN = "CurrentTurn";

        serialize(universe.currentTurn(), value[PROPERTY_CURRENTTURN]);
        serialize(universe.game().players(), value[PROPERTY_PLAYERS]);
        serialize(universe.starSystems(), value[PROPERTY_STARSYSTEMS]);
        serialize(universe.names().availableStarNames(), value[PROPERTY_AVAILABLESTARNAMES]);
        serialize(universe.names().availablePlayerNames(), value[PROPERTY_AVAILABLEPLAYERNAMES]);
        serialize(universe.game().currentPlayerIndex(), value[PROPERTY_CURRENTPLAYERINDEX]);
    }

    void deserialize(const Json::Value & value, Game::Universe & universe)
    {
        static const char * PROPERTY_PLAYERS = "Players";
        static const char * PROPERTY_STARSYSTEMS = "StarSystems";
        static const char * PROPERTY_AVAILABLESTARNAMES = "AvailableStarNames";
        static const char * PROPERTY_AVAILABLEPLAYERNAMES = "AvailablePlayerNames";
        static const char * PROPERTY_CURRENTPLAYERINDEX = "CurrentPlayer";
        static const char * PROPERTY_CURRENTTURN = "CurrentTurn";

        deserializeValue(value[PROPERTY_CURRENTTURN], universe, &Game::Universe::setCurrentTurn);
        deserializeObject(value[PROPERTY_PLAYERS], universe.game(), &Game::Universe::Game::setPlayers);
        deserializeObject(value[PROPERTY_STARSYSTEMS], universe, &Game::Universe::setStarSystems);
        deserializeObject(value[PROPERTY_AVAILABLESTARNAMES], universe.names(), &Game::Universe::Names::setAvailableStarNames);
        deserializeObject(value[PROPERTY_AVAILABLEPLAYERNAMES], universe.names(), &Game::Universe::Names::setAvailablePlayerNames);
        deserializeValue(value[PROPERTY_CURRENTPLAYERINDEX], universe.game(), &Game::Universe::Game::setCurrentPlayerIndex);
    }

    void serialize(const Game::Player & player, Json::Value & value)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_MONEY = "Money";
        static const char * PROPERTY_SHIPCONFIGS = "ShipConfigs";
        static const char * PROPERTY_COMPONENTS = "Components";
        static const char * PROPERTY_SELECTEDSECTOR = "SelectedSector";
        static const char * PROPERTY_HOMESECTOR = "HomeSector";
        static const char * PROPERTY_KNOWNSYSTEMS = "KnownSystems";

        serialize(player.name(), value[PROPERTY_NAME]);
        serialize(player.money(), value[PROPERTY_MONEY]);
        serialize(player.shipConfigs(), value[PROPERTY_SHIPCONFIGS]);
        serialize(player.components(), value[PROPERTY_COMPONENTS]);
        serialize(player.selectedSector(), value[PROPERTY_SELECTEDSECTOR]);
        serialize(player.homeSector(), value[PROPERTY_HOMESECTOR]);
        serialize(player.knownSystems(), value[PROPERTY_KNOWNSYSTEMS]);
    }

    void deserialize(const Json::Value & value, Game::Player & player)
    {
        static const char * PROPERTY_NAME = "Name";
        static const char * PROPERTY_MONEY = "Money";
        static const char * PROPERTY_SHIPCONFIGS = "ShipConfigs";
        static const char * PROPERTY_COMPONENTS = "Components";
        static const char * PROPERTY_SELECTEDSECTOR = "SelectedSector";
        static const char * PROPERTY_HOMESECTOR = "HomeSector";
        static const char * PROPERTY_KNOWNSYSTEMS = "KnownSystems";

        deserializeObject(value[PROPERTY_NAME], player, &Game::Player::setName);
        deserializeValue(value[PROPERTY_MONEY], player, &Game::Player::setMoney);
        deserializeObject(value[PROPERTY_SHIPCONFIGS], player, &Game::Player::setShipConfigs);
        deserializeObject(value[PROPERTY_COMPONENTS], player, &Game::Player::setComponents);
        deserializeObject(value[PROPERTY_SELECTEDSECTOR], player, &Game::Player::setSelectedSector);
        deserializeObject(value[PROPERTY_HOMESECTOR], player, &Game::Player::setHomeSector);
        deserializeObject(value[PROPERTY_KNOWNSYSTEMS], player, &Game::Player::setKnownSystems);
    }

    /// Object to Json
    ///
    template <class T>
    void serialize(T * valueFrom, Json::Value & valueTo)
    {
        if (valueFrom != NULL)
        {
            serialize(*valueFrom, valueTo);
        }
    }

    /// Json to existing object pointer
    ///
    template <class T>
    void deserialize(const Json::Value & valueFrom, T * existingObject)
    {
        if (existingObject != NULL && valueFrom.type() != Json::nullValue)
        {
            deserialize(valueFrom, *existingObject);
        }
    }

    /// Json to new object reference
    ///
    template <class T, class U>
    void deserializeObject(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(U *))
    {
        if (valueFrom.type() != Json::nullValue)
        {
            U * object = new U();
            deserialize(valueFrom, *object);
            (parent.*(setFunction))(object);
        }
    }

    /// Json to new object reference
    ///
    template <class T, class U>
    void deserializeObject(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(const U &))
    {
        if (valueFrom.type() != Json::nullValue)
        {
            U object;
            deserialize(valueFrom, object);
            (parent.*(setFunction))(object);
        }
    }

    /// Json to value
    ///
    template <class T, class U>
    void deserializeValue(const Json::Value & valueFrom, T & parent, void (T::*setFunction)(U))
    {
        if (valueFrom.type() != Json::nullValue)
        {
            U object;
            deserialize(valueFrom, object);
            (parent.*(setFunction))(object);
        }
    }

    /// Value to Json
    ///
    template <class T>
    void serialize(T valueFrom, Json::Value & valueTo)
    {
        valueTo = valueFrom;
    }

    /// std::vector of values to Json array
    ///
    template <typename T>
    void serialize(const std::vector<T> & vector, Json::Value & parent)
    {
        for (typename std::vector<T>::const_iterator it = vector.begin(); it != vector.end(); ++it)
        {
            Json::Value value(Json::objectValue);
            serialize(*it, value);
            parent.append(value);
        }
    }

    /// Json array to std::vector of values
    ///
    template <class T>
    void deserialize(const Json::Value & value, std::vector<T> & vector)
    {
        for (Json::Value::iterator it = value.begin(); it != value.end(); ++it)
        {
            T t;
            deserialize(*it, t);
            vector.push_back(t);
        }
    }

    /// std::vector of objects to Json array
    ///
    template <typename T>
    void serialize(const std::vector<T *> & vector, Json::Value & parent)
    {
        for (typename std::vector<T *>::const_iterator it = vector.begin(); it != vector.end(); ++it)
        {
            Json::Value value(Json::objectValue);
            serialize(*(*it), value);
            parent.append(value);
        }
    }

    /// Json array to std::vector of objects
    ///
    template <class T>
    void deserialize(const Json::Value & value, std::vector<T *> & vector)
    {
        for (Json::Value::iterator it = value.begin(); it != value.end(); ++it)
        {
            T * t = new T();
            deserialize(*it, *t);
            vector.push_back(t);
        }
    }

    template <class T>
    void save(const std::string & filename, const std::vector<T> & data)
    {
        static const char * PROPERTY_CONTENT = "Content";

        Json::Value rootValue(Json::objectValue);
        rootValue["Version"] = 1.0;
        rootValue["Author"] = "Patrick Pelletier";
        rootValue[PROPERTY_CONTENT] = Json::Value(Json::arrayValue);
        serialize(data, rootValue[PROPERTY_CONTENT]);
        Json::StyledWriter writer;
        std::string result = writer.write(rootValue);
        std::ofstream myfile;
        myfile.open(filename.c_str(), std::ios::trunc);
        myfile << result;
        myfile.close();
    }

    template <class T>
    void save(const std::string & filename, const T & data)
    {
        static const char * PROPERTY_CONTENT = "Content";

        Json::Value rootValue(Json::objectValue);
        rootValue["Version"] = 1.0;
        rootValue["Author"] = "Patrick Pelletier";
        rootValue[PROPERTY_CONTENT] = Json::Value(Json::objectValue);
        serialize(data, rootValue[PROPERTY_CONTENT]);
        Json::StyledWriter writer;
        std::string result = writer.write(rootValue);
        std::ofstream myfile;
        myfile.open(filename.c_str(), std::ios::trunc);
        myfile << result;
        myfile.close();
    }

    template <class T>
    void load(const std::string & filename, T & data)
    {
        static const char * PROPERTY_CONTENT = "Content";

        Json::Value rootValue(Json::objectValue);
        std::ifstream myfile;
        std::ostringstream result;
        myfile.open(filename.c_str());
        while (myfile.good())
        {
            std::string line;
            std::getline(myfile, line);
            result << line << std::endl;
        }
        myfile.close();
        Json::Reader reader;
        reader.parse(result.str(), rootValue);
        deserialize(rootValue[PROPERTY_CONTENT], data);
    }
}

void Data::AssetSerializer::save(const std::string & filename, const std::vector<Game::Component> & data)
{
    ::save(filename, data);
}

void Data::AssetSerializer::load(const std::string & filename, std::vector<Game::Component> & data)
{
    ::load(filename, data);
}

void Data::AssetSerializer::save(const std::string & filename, const Game::Technology & data)
{
    ::save(filename, data);
}

void Data::AssetSerializer::load(const std::string & filename, Game::Technology & data)
{
    ::load(filename, data);
}

void Data::AssetSerializer::save(const std::string & filename, const Game::Universe & data)
{
    ::save(filename, data);
}

void Data::AssetSerializer::load(const std::string & filename, Game::Universe & data)
{
    //@todo review 1
    bool before = data.blockNotifications(true);
    ::load(filename, data);
    data.blockNotifications(before);
}

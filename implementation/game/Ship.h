#ifndef SHIP_H
#define SHIP_H

#include "ShipConfig.h"
#include "SectorReference.h"

#include <string>

namespace Game
{
    class Sector;
    class Player;
    class UniverseVisitor;

    class Ship
    {

    public:

        ~Ship();

        Ship();

        Ship(Sector * sector, const ShipConfig & config);

        const ShipConfig & config() const;

        void setConfig(const ShipConfig & config);

        const std::string & name() const;

        void setName(const std::string & name);

        Player * player() const;

        void setPlayer(Player * player);

        std::string playerName() const;

        void setPlayerName(const std::string & playerName);

        Sector * sector() const;

        void setSector(Sector * sector);

        float population() const;

        void setPopulation(float population);

        unsigned int movement() const;

        void setMovement(unsigned int movement);

        unsigned int maximumMovement() const;

        const SectorReference & destination() const;

        void setDestination(const SectorReference & destination);

        unsigned int delayTurns() const;

        void setDelayTurns(unsigned int delayTurns);

        unsigned int arrivalTurns() const;

        void setArrivalTurns(unsigned int arrivalTurns);

        bool canColonize() const;

        void colonize();

        bool canUnload() const;

        void unload();

        bool canLoad() const;

        void load();

        bool canMoveTo(Sector * sector) const;

        void moveTo(Sector * sector);

        bool isInTransit() const;

        void setInTransit(bool inTransit);

        unsigned int hitPoints() const;

        unsigned int damage() const;

        void repair(unsigned int hitPoints);

        void damage(unsigned int hitPoints);

        void accept(UniverseVisitor * visitor);

    private:

        void initFromConfig(const ShipConfig & config);

        ShipConfig mConfig;

        std::string mName;

        Sector * mSector;

        std::string mPlayerName;

        Player * mPlayer;

        bool mInTransit;

        SectorReference mDestination;

        unsigned int mDelayTurns;

        unsigned int mArrivalTurns;

        float mPopulation;

        unsigned int mMovement;

        Ship(const Ship & other);
        
        Ship & operator =(const Ship & other);
        
    };
}

#endif
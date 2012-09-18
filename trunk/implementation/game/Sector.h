#ifndef SECTOR_H
#define SECTOR_H

#include <vector>
#include <set>
#include <map>
#include <string>

#include "Elements.h"

namespace Game
{
    class Star;
    class Planet;
    class Warp;
    class Ship;
    class StarSystem;
    class Player;
    class UniverseVisitor;

    class Sector
    {

    public:

        typedef unsigned char Content;

        enum Contents
        {
            NoContents          = 0x00,
            HasStar             = 0x01,
            HasPlanet           = 0x02,
            HasWarp             = 0x04,
            HasElements         = 0x08,
            HasShips            = 0x10,
            HasShipsInTransit   = 0x20
            //Future1         = 0x40
            //Future2         = 0x80
        };

        class Subscriber
        {

        public:

            virtual ~Subscriber();

            Subscriber(Sector * sector);

            virtual void shipAdded(Ship *);

            virtual void shipRemoved(Ship *);

            virtual void planetSet(Planet *);

            virtual void planetUnset(Planet *);

            virtual void warpSet(Warp *);

            virtual void warpUnset(Warp *);

            virtual void starSet(Star *);

            virtual void starUnset(Star *);

            virtual void shipInTransitAdded(Ship *);

            virtual void shipInTransitRemoved(Ship *);

            virtual void selected();

            virtual void deselected();

            virtual void contentsChanged(Content changed, bool forcedRedraw = false);

            Sector * sector() const;

        protected:

            void notify();

        private:

            void unsubscribe();

            friend class Sector;

            Sector * mSector;

            Subscriber(const Subscriber & other);
            
            Subscriber & operator =(const Subscriber & other);
        };

        ~Sector();

        Sector();

        Sector(StarSystem * starSystem);

        StarSystem * starSystem() const;

        void setStarSystem(StarSystem * starSystem);

        int x() const;

        void setX(int x);

        int y() const;

        void setY(int y);

        Content contents() const;

        Star * star() const;

        void setStar(Star * star);

        Planet * planet() const;

        void setPlanet(Planet * planet);

        Warp * warp() const;

        Elements & elements() { return mElements; }

        const Elements & elements() const { return mElements; }

        void setWarp(Warp * warp);

        const std::vector<Ship *> & ships() const;

        void addShips(const std::vector<Ship *> & ships);

        void addShips(const std::vector<Ship *> & ships, bool forceRedraw);

        void addShip(Ship * ship, bool forceRedraw = false);

        void removeShip(Ship * ship, bool forceRedraw = false);

        const std::vector<Ship *> & shipsInTransit() const;

        void addShipsInTransit(const std::vector<Ship *> & shipsInTransit);

        void addShipInTransit(Ship * ship);

        void removeShipFromTransit(Ship * ship);

        std::set<Player *> players() const;

        void notifyChanged(Content changed = NoContents);

        void notifySelected();

        void notifyDeselected();

        void subscribe(Subscriber * subscriber);

        void unsubscribe(Subscriber * subscriber);

        void accept(UniverseVisitor * visitor);

        bool isEmpty() const;

        Sector * nextSectorInPath(Sector * sector) const;

    private:

        void _addShip(Ship * ship);

        Game::StarSystem * mStarSystem;

        int mX;

        int mY;

        Star * mStar;

        Planet * mPlanet;

        Warp * mWarp;

        Elements mElements;

        std::vector<Ship *> mShips;

        std::vector<Ship *> mShipsInTransit;

        std::set<Subscriber *> mSubscribers;

        Sector(const Sector & other);
        
        Sector & operator =(const Sector & other);
        
    };
}

#endif
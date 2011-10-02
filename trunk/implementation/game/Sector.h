#ifndef SECTOR_H
#define SECTOR_H

#include <vector>
#include <set>
#include <map>
#include <string>

namespace Game
{
    class Star;
    class Planet;
    class Ship;
    class StarSystem;
    class Player;
    class UniverseVisitor;

    class Sector
    {

    public:

        class Subscriber
        {

        public:

            virtual ~Subscriber();

            Subscriber(Sector * sector);

            virtual void shipAdded(Ship *);

            virtual void shipRemoved(Ship *);

            virtual void planetAdded(Planet *);

            virtual void planetRemoved(Planet *);

            virtual void starSet(Star *);

            virtual void starUnset(Star *);

            virtual void shipInTransitAdded(Ship *);

            virtual void shipInTransitRemoved(Ship *);

            virtual void selected();

            virtual void deselected();

            virtual void contentsChanged(bool forcedRedraw = false);

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

        Star * star() const;

        void setStar(Star * star);

        const std::vector<Planet *> & planets() const;

        void setPlanets(const std::vector<Planet *> & planets);

        void addPlanet(Planet * planet);

        void removePlanet(Planet * planet);

        const std::vector<Ship *> & ships() const;

        void setShips(const std::vector<Ship *> & ships);

        void addShip(Ship * ship, bool forceRedraw = false);

        void removeShip(Ship * ship, bool forceRedraw = false);

        const std::vector<Ship *> & shipsInTransit() const;

        void setShipsInTransit(const std::vector<Ship *> & shipsInTransit);

        void addShipInTransit(Ship * ship);

        void removeShipFromTransit(Ship * ship);

        std::set<Player *> players() const;

        void notifyChanged();

        void notifySelected();

        void notifyDeselected();

        void subscribe(Subscriber * subscriber);

        void unsubscribe(Subscriber * subscriber);

        void accept(UniverseVisitor * visitor);

        bool isEmpty() const;

        Sector * nextSectorInPath(Sector * sector) const;

        std::vector<Planet *> planets(Player * player) const;

    private:

        Game::StarSystem * mStarSystem;

        int mX;

        int mY;

        Star * mStar;

        std::vector<Planet *> mPlanets;

        std::vector<Ship *> mShips;

        std::vector<Ship *> mShipsInTransit;

        std::set<Subscriber *> mSubscribers;

        Sector(const Sector & other);
        
        Sector & operator =(const Sector & other);
        
    };
}

#endif
#ifndef SHIPYARD_H
#define SHIPYARD_H

#include <vector>
#include "Sector.h"

namespace Game
{
    class Planet;
    class Ship;
    class UniverseVisitor;

    class Shipyard
        : private Sector::Subscriber
    {

    public:

        ~Shipyard();

        Shipyard(Planet * planet);

        Planet * planet() const;

        const std::vector<Ship *> & shipQueue() const;

        void addShip(Ship * ship);

        void removeShip(Ship * ship);

        void accept(UniverseVisitor * visitor);

    private:

        void shipAdded(Ship * ship);

        void shipRemoved(Ship * ship);

        Planet * mPlanet;

        std::vector<Ship *> mShipQueue;
        
        Shipyard();

        Shipyard(const Shipyard & other);
        
        Shipyard & operator =(const Shipyard & other);
        
    };
}

#endif
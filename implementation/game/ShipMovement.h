#ifndef SHIPMOVEMENT_H
#define SHIPMOVEMENT_H

#include <vector>

namespace Game
{
    class Sector;
    class Ship;

    class ShipMovement
    {

    public:

        ~ShipMovement();

        ShipMovement();

        void addShip(Ship * ship);

        void addShips(const std::vector<Ship *> & ships);

        void setDestination(Sector * sector);

        void run();

    private:

		std::vector<Ship *> mShips;

        ShipMovement(const ShipMovement & other);

        ShipMovement & operator =(const ShipMovement & other);

    };
}

#endif
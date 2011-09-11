#ifndef PLANET_H
#define PLANET_H

#include <string>

namespace Game
{
    class Player;
    class Sector;
    class Shipyard;
    class UniverseVisitor;

    class Planet
    {

    public:

        enum Atmosphere
        {
            None = 0,

            OxygenNitrogen = 1,

            Argon = 2,

            CarbonDioxyde = 3,

            Hydrogen = 4
        };

        enum Size
        {
            Large = 0,

            Medium = 1,

            Small = 2,
        };

        ~Planet();

        Planet();

        Planet(Sector * sector);

        Sector * sector() const;

        void setSector(Sector * sector);

        const std::string & name() const;

        void setName(const std::string & name);

        Player * player() const;

        void setPlayer(Player * player);

        std::string playerName() const;

        void setPlayerName(const std::string & playerName);

        float population() const;

        void setPopulation(float population);

        Atmosphere atmosphere() const;

        void setAtmosphere(Atmosphere atmosphere);

        int atmosphereIndex() const;

        void setAtmosphereIndex(int atmosphere);

        Size size() const;

        void setSize(Size size);

        int sizeIndex() const;

        void setSizeIndex(int size);

        Shipyard * shipyard() const;

        void setShipyard(Shipyard * shipyard);

        void accept(UniverseVisitor * visitor);

    private:

        Sector * mSector;
        
        std::string mName;

        float mPopulation;

        Shipyard * mShipyard;

        std::string mPlayerName;
        
        Player * mPlayer;

        Atmosphere mAtmosphere;

        Size mSize;

        Planet(const Planet & other);
        
        Planet & operator =(const Planet & other);
        
    };
}

#endif
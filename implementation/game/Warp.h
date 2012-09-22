#ifndef WARP_H
#define WARP_H

#include "SectorReference.h"

namespace Game
{
    class Sector;
    class UniverseVisitor;

    class Warp
    {

    public:

        static bool connect(Sector * from, Sector * to);

        ~Warp();

        Warp();

        Warp(Sector * sector);

        Sector * sector() const;

        void setSector(Sector * sector);

        const SectorReference & destination() const;

        void setDestination(const SectorReference & destination);

        void accept(UniverseVisitor * visitor);

    private:
        
        Sector * mSector;

        SectorReference mDestination;

        Warp(const Warp & other);
        
        Warp & operator =(const Warp & other);
        
    };
}

#endif
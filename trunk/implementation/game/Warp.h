#ifndef WARP_H
#define WARP_H

namespace Game
{
    class Sector;
    class UniverseVisitor;

    class Warp
    {

    public:

        ~Warp();

        Warp();

        Warp(Sector * sector);

        Sector * sector() const;

        void setSector(Sector * sector);

        void accept(UniverseVisitor * visitor);

    private:
        
        Sector * mSector;

        Warp(const Warp & other);
        
        Warp & operator =(const Warp & other);
        
    };
}

#endif
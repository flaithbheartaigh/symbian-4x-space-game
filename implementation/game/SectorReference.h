#ifndef SECTORREFERENCE_H
#define SECTORREFERENCE_H

#include <string>

namespace Game
{
    class Universe;
    class StarSystem;
    class Sector;

    class SectorReference
    {

    public:

        ~SectorReference();

        SectorReference();

        SectorReference(Sector * sector);

        SectorReference(StarSystem * starSystem, int x, int y);

        SectorReference(const SectorReference & other);

        SectorReference & operator =(const SectorReference & other);

        bool operator ==(const SectorReference& other) const;
        
        bool isValid() const;

        Sector * sector() const;

        const std::string & name() const;

        void setName(const std::string & name);

        int x() const;

        void setX(int x);

        int y() const;

        void setY(int y);

    private:

        Universe * mUniverse;

        std::string mName;

        int mX;

        int mY;

        mutable Sector * mCached;
    };
}

#endif
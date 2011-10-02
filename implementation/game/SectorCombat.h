#ifndef SECTORCOMBAT_H
#define SECTORCOMBAT_H

namespace Game
{
    class Sector;

    class SectorCombat
    {

    public:

        ~SectorCombat();

        SectorCombat(Sector * sector);

        bool canRun() const;

        void run();

    private:

        Sector * mSector;

        SectorCombat();

        SectorCombat(const SectorCombat & other);

        SectorCombat & operator =(const SectorCombat & other);

    };
}

#endif
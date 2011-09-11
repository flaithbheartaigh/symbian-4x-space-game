#ifndef STATSVISITOR_H
#define STATSVISITOR_H

#include "UniverseVisitor.h"

#include <vector>

namespace Game
{
    class StatsVisitor
        : public UniverseVisitor
    {

    public:

        ~StatsVisitor();

        StatsVisitor(Player * player);

        void visit(Universe * universe);
        
        void visit(StarSystem * starSystem);
        
        void visit(Sector * sector);
        
        void visit(Star * star);
        
        void visit(Planet * planet);
        
        void visit(Ship * ship);
       
        void visit(Shipyard * shipyard);

        void visit(Player * player);

        std::vector<Planet *> mPlanets;

        std::vector<Ship *> mShips;

        std::vector<Shipyard *> mShipyards;

        int mPopulation;

    private:

        Player * mPlayer;

        StatsVisitor();

        StatsVisitor(const StatsVisitor & other);
        
        StatsVisitor & operator =(const StatsVisitor & other);
        
    };
}

#endif
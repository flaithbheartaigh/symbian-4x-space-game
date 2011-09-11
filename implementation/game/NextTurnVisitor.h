#ifndef NEXTTURNVISITOR_H
#define NEXTTURNVISITOR_H

#include "UniverseVisitor.h"

#include <map>
#include <vector>

namespace Game
{
    class Player;
    class Planet;

    class NextTurnVisitor
        : public UniverseVisitor
    {

    public:

        ~NextTurnVisitor();

        NextTurnVisitor();

        void visit(Universe * universe);
        
        void visit(StarSystem * starSystem);
        
        void visit(Sector * sector);
        
        void visit(Star * star);
        
        void visit(Planet * planet);
        
        void visit(Ship * ship);
       
        void visit(Shipyard * shipyard);

        void visit(Player * player);

    private:

        NextTurnVisitor(const NextTurnVisitor & other);
        
        NextTurnVisitor & operator =(const NextTurnVisitor & other);
        
    };
}

#endif
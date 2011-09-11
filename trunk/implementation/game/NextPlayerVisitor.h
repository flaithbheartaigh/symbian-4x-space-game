#ifndef NEXTPLAYERVISITOR_H
#define NEXTPLAYERVISITOR_H

#include "UniverseVisitor.h"

namespace Game
{
    class NextPlayerVisitor
        : public UniverseVisitor
    {

    public:

        ~NextPlayerVisitor();

        NextPlayerVisitor();

        void visit(Universe * universe);
        
        void visit(StarSystem * starSystem);
        
        void visit(Sector * sector);
        
        void visit(Star * star);
        
        void visit(Planet * planet);
        
        void visit(Ship * ship);
       
        void visit(Shipyard * shipyard);

        void visit(Player * player);

        void visit(StarDriveModule * starDriveModule);

        void visit(EngineModule * engineModule);

        void visit(ColonyModule * colonyModule);

    private:
        
        NextPlayerVisitor(const NextPlayerVisitor & other);
        
        NextPlayerVisitor & operator =(const NextPlayerVisitor & other);
        
    };
}

#endif
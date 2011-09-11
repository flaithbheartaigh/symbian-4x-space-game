#ifndef DESERIALIZEVISITOR_H
#define DESERIALIZEVISITOR_H

#include "UniverseVisitor.h"

namespace Game
{
    class DeserializeVisitor
        : public UniverseVisitor
    {

    public:

        ~DeserializeVisitor();

        DeserializeVisitor();

        void visit(Universe * universe);
        
        void visit(StarSystem * starSystem);
        
        void visit(Sector * sector);
        
        void visit(Star * star);
        
        void visit(Planet * planet);
        
        void visit(Ship * ship);
       
        void visit(Shipyard * shipyard);

        void visit(Player * player);

    private:

        Sector * mSector;
        
        DeserializeVisitor(const DeserializeVisitor & other);
        
        DeserializeVisitor & operator =(const DeserializeVisitor & other);
        
    };
}

#endif
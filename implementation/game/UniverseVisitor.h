#ifndef UNIVERSEVISITOR_H
#define UNIVERSEVISITOR_H

#include <string>

namespace Game
{
    class Universe;
    class StarSystem;
    class Sector;
    class Star;
    class Planet;
    class Warp;
    class Ship;
    class Shipyard;
    class Player;
    
    class UniverseVisitor
    {

    public:

        virtual ~UniverseVisitor();

        UniverseVisitor();

        virtual void visit(Universe * universe);
        
        virtual void visit(StarSystem * starSystem);
        
        virtual void visit(Sector * sector);
        
        virtual void visit(Star * star);
        
        virtual void visit(Planet * planet);
        
        virtual void visit(Warp * warp);

        virtual void visit(Ship * ship);
       
        virtual void visit(Shipyard * shipyard);

        virtual void visit(Player * player);

    private:
    
        UniverseVisitor(const UniverseVisitor & other);
        
        UniverseVisitor & operator =(const UniverseVisitor & other);
        
    };
}

#endif
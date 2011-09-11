#ifndef STAR_H
#define STAR_H

#include <string>

namespace Game
{
    class Sector;
    class UniverseVisitor;

    class Star
    {

    public:

        ~Star();

        Star();

        Star(Sector * sector);

        Sector * sector() const;

        void setSector(Sector * sector);

        const std::string & name() const;

        void setName(const std::string & name);

        void accept(UniverseVisitor * visitor);

    private:
        
        Sector * mSector;

        std::string mName;

        Star(const Star & other);
        
        Star & operator =(const Star & other);
        
    };
}

#endif
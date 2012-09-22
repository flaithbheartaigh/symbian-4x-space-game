#ifndef STARSYSTEM_H
#define STARSYSTEM_H

#include <vector>
#include <set>
#include <string>

namespace Game
{
    class Universe;
    class Sector;
    class Player;
    class Warp;
    class UniverseVisitor;
    class DeserializeVisitor;

    class StarSystem
    {

    public:

        class Subscriber
        {

        public:

            virtual ~Subscriber();

            Subscriber(StarSystem * starSystem);

            virtual void sectorAdded(Sector * sector);

            virtual void sectorRemoved(Sector * sector);

            virtual void becameKnown();

            StarSystem * starSystem() const;

        //@todo review 1
        //protected:

            void notify();

        private:

            void unsubscribe();

            friend class StarSystem;

            StarSystem * mStarSystem;

            Subscriber(const Subscriber & other);
            
            Subscriber & operator =(const Subscriber & other);
        };

        static const int Size;

        ~StarSystem();

        StarSystem();

        StarSystem(Universe * universe);

        Universe * universe() const;

        void setUniverse(Universe * universe);

        void generate(unsigned int connections = 2);

        const std::string & name() const;

        void setName(const std::string & name);

        int x() const;

        void setX(int x);

        int y() const;

        void setY(int y);

        const std::vector<Sector *> & sectors() const;

        void setSectors(const std::vector<Sector *> & sectors);

        void addSector(Sector * sector);

        void removeSector(Sector * sector);

        std::set<Player *> players() const;

        std::set<Warp *> warps() const;

        float distance(StarSystem * starSystem) const;

        void notifyBecameKnown();

        void subscribe(Subscriber * subscriber);

        void unsubscribe(Subscriber * subscriber);

        void accept(UniverseVisitor * visitor);

    private:

        //@todo review 1
        friend class DeserializeVisitor;

        Universe * mUniverse;

        std::string mName;

        int mX;

        int mY;

        std::vector<Sector *> mSectors;
        
        std::set<Subscriber *> mSubscribers;

        StarSystem(const StarSystem & other);
        
        StarSystem & operator =(const StarSystem & other);
        
    };
}

#endif
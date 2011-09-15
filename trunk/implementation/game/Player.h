#ifndef PLAYER_H
#define PLAYER_H

#include "Universe.h"
#include "ShipConfig.h"
#include "SectorReference.h"
#include "StarSystemReference.h"
#include <string>
#include <vector>

namespace Game
{
    class AI;
    class Sector;
    class UniverseVisitor;

    class Player
    {

    public:

        class Current
            : private Universe::Game::Subscriber
        {

        public:

            class Subscriber
            {

            public:

                virtual ~Subscriber();

                Subscriber();

                virtual void selectedSectorChanged(Sector * sector);

            private:

                void unsubscribe();

                friend class Player;

                friend class Current;

                Subscriber(const Subscriber & other);
                
                Subscriber & operator =(const Subscriber & other);
            };

            static Current & instance();

            ~Current();

            void setSelectionAllowed(bool allowed);

            bool selectionAllowed() const;

            void subscribe(Subscriber * subscriber);

            void unsubscribe(Subscriber * subscriber);

        private:

            void playerDeactivated(Player * player);

            void playerActivated(Player * player);

            bool mSelectionAllowed;

            std::set<Subscriber *> mSubscribers;

            static Current * _instance;

            friend class Player;

            Current();

            Current(const Current & other);
            
            Current & operator =(const Current & other);
            
        };

        static const int Colors[4][3];

        static std::vector<int> color(int index);

        ~Player();

        Player();

        void update();

        const std::string & name() const;

        void setName(const std::string & name);

        int money() const;

        void setMoney(int money);

        const std::vector<ShipConfig> & shipConfigs() const;

        void setShipConfigs(const std::vector<ShipConfig> & shipConfigs);

        ShipConfig shipConfig(const std::string & configName) const;

        void addShipConfig(const ShipConfig & shipConfig);

        const std::vector<Component> & components() const;

        void setComponents(const std::vector<Component> & components);

        const SectorReference & selectedSector() const;

        void setSelectedSector(const SectorReference & selectedSector);

        const SectorReference & homeSector() const;

        void setHomeSector(const SectorReference & homeSector);

        const std::vector<StarSystemReference> & knownSystems() const;

        void setKnownSystems(const std::vector<StarSystemReference> & knownSystems);

        void addKnownSystem(const StarSystemReference & knownSystem);

        bool knows(StarSystem * starSystem) const;

        void accept(UniverseVisitor * visitor);

        AI * ai() const;

        void setAI(AI * ai);

        bool isHuman() const;

    private:
        
        std::string mName;

        int mMoney;

        std::vector<ShipConfig> mShipConfigs;

        std::vector<Component> mComponents;

        SectorReference mSelectedSector;

        SectorReference mHomeSector;

        std::vector<StarSystemReference> mKnownSystems;

        AI * mAI;

        Player(const Player & other);
        
        Player & operator =(const Player & other);
        
    };
}

#endif
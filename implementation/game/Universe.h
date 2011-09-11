#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <set>
#include <vector>
#include <string>

namespace Game
{
    class StarSystem;
    class Sector;
    class Player;
    class UniverseVisitor;
    class DeserializeVisitor;

    class Universe
    {

    public:

        class Names
        {

        public:

            ~Names();

            Names();
            
            const std::vector<std::string> & availableStarNames() const;

            void setAvailableStarNames(const std::vector<std::string> & starNames);

            std::string nextAvailableStarName();

            const std::vector<std::string> & availablePlayerNames() const;

            void setAvailablePlayerNames(const std::vector<std::string> & playerNames);

            std::string nextAvailablePlayerName();

            void clear();

        private:

            std::vector<std::string> mAvailableStarNames;

            std::vector<std::string> mAvailablePlayerNames;

            Names(const Names & other);
            
            Names & operator =(const Names & other);

        };

        class Game
        {

        public:

            class Subscriber
            {

            public:

                virtual ~Subscriber();

                Subscriber();

                Subscriber(Universe * universe);

                virtual void playerDeactivated(Player * player);

                virtual void playerActivated(Player * player);

                Universe * universe() const;

            protected:

                void notify();

            private:

                void unsubscribe();

                friend class Universe;

                Universe * mUniverse;

                Subscriber(const Subscriber & other);
                
                Subscriber & operator =(const Subscriber & other);
            };

            ~Game();

            Game();

            void setUniverse(Universe * universe);
       
            const std::vector<Player *> & players() const;

            void setPlayers(const std::vector<Player *> & players);

            void addPlayer(Player * player);

            Player * currentPlayer() const;

            Player * player(const std::string & name) const;

            unsigned int playerIndex(Player * player) const;

            void setCurrentPlayerIndex(unsigned int index);

            unsigned int currentPlayerIndex() const;

            bool haveAllPlayersPlayed() const;

            void subscribe(Subscriber * subscriber);

            void unsubscribe(Subscriber * subscriber);

            void clear();

        private:

            Universe * mUniverse;

            std::vector<Player *> mPlayers;

            unsigned int mCurrentPlayerIndex;

            std::set<Subscriber *> mSubscribers;

            Game(const Game & other);
            
            Game & operator =(const Game & other);

        };

        class Subscriber
        {

        public:

            virtual ~Subscriber();

            Subscriber();

            Subscriber(Universe * universe);

            virtual void starSystemAdded(StarSystem * starSystem);

            virtual void starSystemRemoved(StarSystem * starSystem);

            Universe * universe() const;

        //@todo review 1
        //protected:

            void notify();

        private:

            void unsubscribe();

            friend class Universe;

            Universe * mUniverse;

            Subscriber(const Subscriber & other);
            
            Subscriber & operator =(const Subscriber & other);
        };

        static Universe & instance();

        ~Universe();

        void update();

        void setCurrentTurn(unsigned int turn);

        unsigned int currentTurn() const;

        const std::vector<StarSystem *> & starSystems() const;

        void setStarSystems(const std::vector<StarSystem *> & starSystems);

        void addStarSystem(StarSystem * starSystem);

        void removeStarSystem(StarSystem * starSystem);

        void subscribe(Subscriber * subscriber);

        void unsubscribe(Subscriber * subscriber);

        void accept(UniverseVisitor * visitor);

        const Names & names() const;

        const Game & game() const;

        Names & names();

        Game & game();

        void generate();

        bool notificationsBlocked() const;

        bool blockNotifications(bool block);

        void clear();

    private:

        //@todo review 1
        friend class DeserializeVisitor;

        unsigned int mCurrentTurn;

        std::vector<StarSystem *> mStarSystems;

        std::set<Subscriber *> mSubscribers;

        Names mNames;

        Game mGame;

        bool mNotificationsBlocked;

        static Universe * _instance;
        
        Universe();
        
        Universe(const Universe & other);
        
        Universe & operator =(const Universe & other);
        
    };
}

#endif
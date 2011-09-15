#ifndef AI_H
#define AI_H

#include <string>

namespace Game
{
    class Player;

    class AI
    {

    public:

        virtual ~AI();

        AI();

        AI(Player * player);

        Player * player() const;

        void setPlayer(Player * player);
		
		virtual void run();

    private:
        
        Player * mPlayer;

        AI(const AI & other);
        
        AI & operator =(const AI & other);
        
    };

    class NPC
        : public AI
    {

    public:

        ~NPC();

        NPC();

        NPC(Player * player);

		void run();

    private:
        
        NPC(const NPC & other);
        
        NPC & operator =(const NPC & other);
        
    };
}

#endif
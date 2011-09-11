#ifndef AI_H
#define AI_H

#include <string>

namespace Game
{
    class Player;

    class AI
    {

    public:

        ~AI();

        AI();

        AI(Player * player);

        Player * player() const;

        void setPlayer(Player * player);
		
		void run();

    private:
        
        Player * mPlayer;

        AI(const AI & other);
        
        AI & operator =(const AI & other);
        
    };
}

#endif
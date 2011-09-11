#ifndef EVENTS_H
#define EVENTS_H

#include "Event.h"

#include <set>
#include <vector>

namespace Game
{
    class Events
    {

    public:

        class Listener
        {

        public:

            virtual ~Listener();

            Listener();

            virtual void listenEvent(const Event & event) = 0;

        private:

            Listener(const Listener & other);
            
            Listener & operator =(const Listener & other);
        };

        static Events & instance();

        ~Events();

        void addListener(Listener * listener);

        void removeListener(Listener * listener);

        void add(const Event & event);

        Event next();

    private:

        std::set<Listener *> mListeners;

        Event mNextUniqueEvent;

        static Events * _instance;

        Events();

        Events(const Events & other);
        
        Events & operator =(const Events & other);
        
    };
}

#endif
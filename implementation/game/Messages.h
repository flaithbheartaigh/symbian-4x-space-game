#ifndef MESSAGES_H
#define MESSAGES_H

#include <set>
#include <deque>
#include <string>

namespace Game
{
    class Messages
    {

    public:

        class Subscriber
        {

        public:

            virtual ~Subscriber();

            Subscriber();

            virtual void messagePosted(const std::string & message) = 0;

        private:

            Subscriber(const Subscriber & other);
            
            Subscriber & operator =(const Subscriber & other);
        };

        static Messages & instance();

        ~Messages();

        void addSubscriber(Subscriber * subscriber);

        void removeSubscriber(Subscriber * subscriber);

        void post(const std::string & message);

    private:

        std::set<Subscriber *> mSubscribers;
		
		std::deque<std::string> mMessages;

        static Messages * _instance;

        Messages();

        Messages(const Messages & other);
        
        Messages & operator =(const Messages & other);
        
    };
}

#endif
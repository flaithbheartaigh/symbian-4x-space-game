#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace Game
{
    class Component
    {

    public:

        enum Type
        {
            None = 0,

            StarDrive = 1,

            Colony = 2,

            Engine = 3,

            Weapon = 4

        };

        static std::string typeToString(Type type);

        static Type stringToType(const std::string & string);

        ~Component();

        Component();

        Component(const std::string & name, Component::Type type, unsigned int level, unsigned int hitPoints, int cost);

        Component(const Component & other);
        
        Component & operator =(const Component & other);

        const std::string & name() const;

        void setName(const std::string & name);

        Component::Type type() const;

        int typeIndex() const;

        void setType(Component::Type type);

        void setTypeIndex(int type);

        unsigned int level() const;

        void setLevel(unsigned int level);

        unsigned int hitPoints() const;

        void setHitPoints(unsigned int hitPoints);

        unsigned int damage() const;

        void setDamage(unsigned int damage);

        unsigned int remain() const;

        void setRemain(unsigned int remain);

        bool destroyed() const;

        int cost() const;

        void setCost(int cost);

    private:

        std::string mName;

        Component::Type mType;

        unsigned int mLevel;

        unsigned int mHitPoints;

        unsigned int mDamage;

        int mCost;

    };
}

#endif
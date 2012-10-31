#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "Elements.h"

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

            Weapon = 4,

            Hull = 5,

            Collector = 6

        };

        static std::string typeToString(Type type);

        static Type stringToType(const std::string & string);

        ~Component();

        Component();

        Component(const Component & other);
        
        Component & operator =(const Component & other);

        bool operator ==(const Component & other) const;

        const std::string & name() const;

        void setName(const std::string & name);

        Component::Type type() const;

        int typeIndex() const;

        void setType(Component::Type type);

        void setTypeIndex(int type);

        float value() const;

        void setValue(float value);

        unsigned int hitPoints() const;

        void setHitPoints(unsigned int hitPoints);

        unsigned int damage() const;

        void setDamage(unsigned int damage);

        unsigned int remain() const;

        void setRemain(unsigned int remain);

        bool destroyed() const;

        int cost() const;

        void setCost(int cost);

        Elements & elements() { return mElements; }

        const Elements & elements() const { return mElements; }

    private:

        std::string mName;

        Component::Type mType;

        float mValue;

        unsigned int mHitPoints;

        unsigned int mDamage;

        int mCost;

        Elements mElements;

    };
}

#endif
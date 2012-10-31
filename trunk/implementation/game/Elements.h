#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <string>

namespace Game
{
    class Elements
    {

    public:

        ~Elements();

        Elements();

        Elements(const Elements & other);
        
        Elements & operator =(const Elements & other);

        bool operator ==(const Elements & other) const;

        unsigned int Hydrogen;

        unsigned int Uranium;

        unsigned int AntiHydrogen;

        bool isEmpty() const { return Hydrogen == 0 && Uranium == 0 && AntiHydrogen == 0; }

        std::string toString() const;
    };
}

#endif
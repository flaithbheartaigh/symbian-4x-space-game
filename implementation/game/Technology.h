#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include "Component.h"

#include <vector>
#include <string>

namespace Game
{
    class Technology
    {

    public:

        static Technology & instance();

        ~Technology();

        const std::vector<Component> & components() const;

        void setComponents(const std::vector<Component> & components);

    private:

        std::vector<Component> mComponents;

        static Technology * _instance;

        Technology();

        Technology(const Technology & other);
        
        Technology & operator =(const Technology & other);
        
    };
}

#endif
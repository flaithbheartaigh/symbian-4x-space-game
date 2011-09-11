#ifndef SHIPCONFIG_H
#define SHIPCONFIG_H

#include "Component.h"

#include <vector>
#include <string>

namespace Game
{
    class ShipConfig
    {

    public:

        ~ShipConfig();

        ShipConfig();

        ShipConfig(const ShipConfig & other);
        
        ShipConfig & operator =(const ShipConfig & other);

        const std::string & name() const;

        void setName(const std::string & name);

        const std::vector<Component> & components() const;

        std::vector<Component> & components();

        void setComponents(const std::vector<Component> & components);

        bool has(Component::Type type) const;

        std::vector<Component> components(Component::Type type) const;

        int highestLevel(Component::Type type) const;

        int lowestLevel(Component::Type type) const;

        int cost() const;

    private:

        std::string mName;

        std::vector<Component> mComponents;
    };
}

#endif
#ifndef ASSETSERIALIZER_H
#define ASSETSERIALIZER_H

#include <vector>
#include <string>

namespace Game
{
    class Parameters;
    class Component;
    class Universe;
}

namespace Data
{
    namespace AssetSerializer
    {
        void save(const std::string & filename, const std::vector<Game::Component> & data);

        void load(const std::string & filename, std::vector<Game::Component> & data);
        
        void save(const std::string & filename, const Game::Parameters & data);

        void load(const std::string & filename, Game::Parameters & data);

        void save(const std::string & filename, const Game::Universe & data);

        void load(const std::string & filename, Game::Universe & data);

    }
}

#endif
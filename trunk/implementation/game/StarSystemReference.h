#ifndef STARSYSTEMREFERENCE_H
#define STARSYSTEMREFERENCE_H

#include <string>

namespace Game
{
    class StarSystem;
    class Universe;

    class StarSystemReference
    {

    public:

        ~StarSystemReference();

        StarSystemReference();

        StarSystemReference(StarSystem * starSystem);

        StarSystemReference(Universe * universe, const std::string & name);

        StarSystemReference(Universe * universe, int x, int y);

        StarSystemReference(const StarSystemReference & other);

        StarSystemReference & operator =(const StarSystemReference & other);

        bool operator ==(const StarSystemReference& other) const;

        bool isValid() const;

        StarSystem * starSystem() const;

        const std::string & name() const;

        void setName(const std::string & name);

    private:

        Universe * mUniverse;

        std::string mName;

        mutable StarSystem * mCached;
    };
}

#endif
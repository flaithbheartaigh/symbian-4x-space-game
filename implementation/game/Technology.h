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

        class ColonyModule
        {

        public:

            ~ColonyModule();

            ColonyModule();

            ColonyModule(const ColonyModule & other);
            
            ColonyModule & operator =(const ColonyModule & other);

            float population() const;

            void setPopulation(float population);

            const Component & component() const;

            Component & component();

        private:

            Component mComponent;

            float mPopulation;

        };

        class StarDriveModule
        {

        public:

            ~StarDriveModule();

            StarDriveModule();

            StarDriveModule(const StarDriveModule & other);
            
            StarDriveModule & operator =(const StarDriveModule & other);

            float travelSpeed() const;

            void setTravelSpeed(float travelSpeed);

            unsigned int arrival(float distance) const;

            const Component & component() const;

            Component & component();

        private:

            Component mComponent;

            float mTravelSpeed;

        };

        class EngineModule
        {

        public:

            ~EngineModule();

            EngineModule();

            EngineModule(const EngineModule & other);
            
            EngineModule & operator =(const EngineModule & other);

            unsigned int speed() const;

            void setSpeed(unsigned int speed);

            const Component & component() const;

            Component & component();

        private:

            Component mComponent;

            unsigned int mSpeed;

        };

        class WeaponModule
        {

        public:

            ~WeaponModule();

            WeaponModule();

            WeaponModule(const WeaponModule & other);
            
            WeaponModule & operator =(const WeaponModule & other);

            unsigned int damage() const;

            void setDamage(unsigned int damage);

            const Component & component() const;

            Component & component();

        private:

            Component mComponent;

            unsigned int mDamage;

        };

        static Technology & instance();

        ~Technology();

        const std::vector<ColonyModule> & colonyModules() const;

        void setColonyModules(const std::vector<ColonyModule> & colonyModules);

        const std::vector<StarDriveModule> & starDriveModules() const;

        void setStarDriveModules(const std::vector<StarDriveModule> & starDriveModules);

        const std::vector<EngineModule> & engineModules() const;

        void setEngineModules(const std::vector<EngineModule> & engineModules);

        const std::vector<WeaponModule> & weaponModules() const;

        void setWeaponModules(const std::vector<WeaponModule> & weaponModules);

    private:

        std::vector<ColonyModule> mColonyModules;

        std::vector<StarDriveModule> mStarDriveModules;

        std::vector<EngineModule> mEngineModules;

        std::vector<WeaponModule> mWeaponModules;

        static Technology * _instance;

        Technology();

        Technology(const Technology & other);
        
        Technology & operator =(const Technology & other);
        
    };
}

#endif
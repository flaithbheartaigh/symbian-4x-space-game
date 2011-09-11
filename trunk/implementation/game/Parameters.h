#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <string>

namespace Game
{
    class Parameters
    {

    public:

        class ColonyModule
        {

        public:

            ~ColonyModule();

            ColonyModule();

            ColonyModule(float population);

            ColonyModule(const ColonyModule & other);
            
            ColonyModule & operator =(const ColonyModule & other);

            float population() const;

            void setPopulation(float population);

        private:

            float mPopulation;

        };

        class StarDriveModule
        {

        public:

            ~StarDriveModule();

            StarDriveModule();

            StarDriveModule(unsigned int delayTurns, float travelSpeed);

            StarDriveModule(const StarDriveModule & other);
            
            StarDriveModule & operator =(const StarDriveModule & other);

            unsigned int delayTurns() const;

            void setDelayTurns(unsigned int delayTurns);

            float travelSpeed() const;

            void setTravelSpeed(float travelSpeed);

            unsigned int arrivalTurns(float distance) const;

        private:

            unsigned int mDelayTurns;

            float mTravelSpeed;

        };

        class EngineModule
        {

        public:

            ~EngineModule();

            EngineModule();

            EngineModule(unsigned int speed);

            EngineModule(const EngineModule & other);
            
            EngineModule & operator =(const EngineModule & other);

            unsigned int speed() const;

            void setSpeed(unsigned int speed);

        private:

            unsigned int mSpeed;

        };

        static Parameters & instance();

        ~Parameters();

        const std::vector<ColonyModule> & colonyModules() const;

        void setColonyModules(const std::vector<ColonyModule> & colonyModules);

        const std::vector<StarDriveModule> & starDriveModules() const;

        void setStarDriveModules(const std::vector<StarDriveModule> & starDriveModules);

        const std::vector<EngineModule> & engineModules() const;

        void setEngineModules(const std::vector<EngineModule> & engineModules);

        void setDataFilesPath(const std::string & dataFilesPath);

        const std::string & getDataFilesPath() const;

        std::string getDataFilePath(const std::string & path) const;

    private:

        std::vector<ColonyModule> mColonyModules;

        std::vector<StarDriveModule> mStarDriveModules;

        std::vector<EngineModule> mEngineModules;

        std::string mDataFilesPath;

        static Parameters * _instance;

        Parameters();

        Parameters(const Parameters & other);
        
        Parameters & operator =(const Parameters & other);
        
    };
}

#endif
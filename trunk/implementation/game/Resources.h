#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>

namespace Game
{
    class Resources
    {

    public:

        static Resources & instance();

        ~Resources();

        void setDataFilesPath(const std::string & dataFilesPath);

        const std::string & getDataFilesPath() const;

        std::string getDataFilePath(const std::string & path) const;

    private:

        std::string mDataFilesPath;

        static Resources * _instance;

        Resources();

        Resources(const Resources & other);
        
        Resources & operator =(const Resources & other);
        
    };
}

#endif
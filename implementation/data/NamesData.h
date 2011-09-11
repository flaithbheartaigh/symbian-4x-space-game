#ifndef NAMESDATA_H
#define NAMESDATA_H

#include <vector>
#include <string>

namespace Data
{
    class NamesData
    {

    public:

        enum Action
        {
            Save,

            Load
        };

        ~NamesData();

        NamesData(const std::string & filename, std::vector<std::string> * names, Action action);

    private:

        NamesData();
        
        NamesData(const NamesData & other);
        
        NamesData & operator =(const NamesData & other);
        
    };
}

#endif
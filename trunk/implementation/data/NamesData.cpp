// Symbian 4X Space Game
// 
// Copyright (C) 2011 Patrick Pelletier
// 
// This program is free software: you can redistribute it and/or modify it
// under the  terms of the GNU  General Public License as published by the
// Free Software Foundation, either version 3 of the License, or any later
// version.
// 
// This  program  is distributed  in  the  hope  that  it will  be  useful,
// but  WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
// 
// You should have received a copy  of the GNU General Public License along
// with this program. See <http://www.opensource.org/licenses/gpl-3.0.html>

#include "NamesData.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>

#include <game/Resources.h>

namespace
{
    static const char * PROPERTY_NAMES = "Names";
    static const char * NAMESPATH = "names/";
}

using namespace Data;

NamesData::~NamesData()
{
    
}

NamesData::NamesData(const std::string & filename, std::vector<std::string> * names, Action action)
{
    std::ostringstream ss;
    ss << Game::Resources::instance().getDataFilePath(std::string(NAMESPATH)) << filename;

    if (names != NULL)
    {
        switch (action)
        {
        case Save:
            {
                Json::Value rootValue(Json::objectValue);
                rootValue["Version"] = 1.0;
                rootValue["Author"] = "Patrick Pelletier";

                {
                    // NamesData
                    Json::Value namesValue(Json::arrayValue);
                    for (std::vector<std::string>::const_iterator it = names->begin(); it != names->end(); ++it)
                    {
                        namesValue.append(Json::Value(*it));
                    }
                    rootValue[PROPERTY_NAMES] = namesValue;
                }

                Json::StyledWriter writer;
                std::string result = writer.write(rootValue);
                
                std::ofstream myfile;
                myfile.open(ss.str().c_str(), std::ios::trunc);
                myfile << result;
                myfile.close();
            }
            break;
        case Load:
            {
                Json::Value rootValue(Json::objectValue);

                std::ifstream myfile;
                std::ostringstream result;
                myfile.open(ss.str().c_str());
                while (myfile.good())
                {
                    std::string line;
                    std::getline(myfile, line);
                    result << line << std::endl;
                }
                myfile.close();

                Json::Reader reader;
                reader.parse(result.str(), rootValue);

                {
                    // Universe
                    Json::Value namesValue = rootValue[PROPERTY_NAMES];
                    for (Json::Value::iterator it = namesValue.begin(); it != namesValue.end(); ++it)
                    {
                        names->push_back((*it).asString());
                    }
                }
            }
            break;
        }
    }
}

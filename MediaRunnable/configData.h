#ifndef _CONFIGDATA_H
#define _CONFIGDATA_H

#include <vector>
#include <string>
#include <fstream>

class ConfigData
{
    public:
        ConfigData(std::string file_name)
        {
            filename = file_name;
        }
        ~ConfigData(){}
        int parse_file();

    public:
        std::string filename;
};

#endif

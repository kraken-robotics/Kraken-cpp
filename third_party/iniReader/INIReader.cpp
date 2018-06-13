// Read an INI file into easy-to-access name/value pairs.

#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "INIReader.h"

#if DEBUG
#include <iostream>

#endif

#include <fstream>

INIReader::INIReader(std::string filename)
{
    parseFile(filename);
}

void INIReader::safeToLower(std::string& stringRef) noexcept
{
    try
    {
        std::transform(stringRef.begin(), stringRef.end(), stringRef.begin(), ::tolower);
    }
    catch (std::bad_alloc& e)
    {
        stringRef = "";
    }
}

void INIReader::parseFile(const std::string& filename)
{
    std::ifstream infile(filename);
    std::string currentSection;

    for( std::string line; getline( infile, line ); )
    {
        //Empty line, skip it
        if(line.length() == 0)
        {
            continue;
        }

        auto firstChar = line[0];
        if(firstChar== ';' || firstChar == '#')
        {
            //skip comment line
            continue;
        }
        else if(firstChar == '[')
        {
            //change section
            auto endPosition = line.find(']') - 1;
            if(endPosition == std::string::npos)
            {
                throw (std::invalid_argument("Malformed INI. Couldn't find end section delimiter ']' at line " + line));
            }
            currentSection = line.substr(1, endPosition);
        }
        else
        {
            //register new key:value pair
            auto separatorPosition = line.find(':');
            if(separatorPosition == std::string::npos)
            {
                separatorPosition = line.find('=');
                if(separatorPosition == std::string::npos)
                {
                    throw (std::invalid_argument("Malformed INI. Incorrect separator in key:value pair at line " + line));
                }
            }

            auto key = makeKey(currentSection, line.substr(0, separatorPosition));
            auto value = line.substr(separatorPosition + 1, line.length());
            values_.emplace(key, value);
        }
    }


}

std::string INIReader::getString(const std::string &section, const std::string &name, std::string default_value) const noexcept
{
    auto key = makeKey(section, name);
    return values_.count(key) ? values_.at(key) : default_value;
}

int INIReader::getInteger(const std::string &section, const std::string &name, int default_value) const noexcept
{
    auto valstr = getString(section, name);
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    try {
        auto parsed = std::stoi(valstr);
        return parsed;
    }
    catch (std::invalid_argument& e)
    {
        return default_value;
    }
}

float INIReader::getReal(const std::string &section, const std::string &name, float default_value) const noexcept
{
    auto valstr = getString(section, name);

    try {
        auto parsed = std::stof(valstr);
        return parsed;
    }
    catch (std::invalid_argument& e)
    {
        return default_value;
    }
}

bool INIReader::getBoolean(const std::string &section, const std::string &name, bool default_value) const noexcept
{
    std::string valstr = getString(section, name);
    // Convert to lower case to make std::string comparisons case-insensitive
    safeToLower(valstr);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
    {
        return true;
    }
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
    {
        return false;
    }
    else
    {
        return default_value;
    }
}

std::string INIReader::makeKey(const std::string &section, const std::string &name) noexcept
{
    std::string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    safeToLower(key);
    return key;
}

template<>
int INIReader::get<int>(const std::string& sectionName, const std::string& name, int default_value) const noexcept
{
    return getInteger(sectionName, name, default_value);
}

template<>
float INIReader::get<float>(const std::string& sectionName, const std::string& name, float default_value) const noexcept
{
    return getReal(sectionName, name, default_value);
}

template<>
bool INIReader::get<bool>(const std::string& sectionName, const std::string& name, bool default_value) const noexcept
{
    return getBoolean(sectionName, name, default_value);
}

template<>
std::string INIReader::get<std::string>(const std::string& sectionName, const std::string& name,
                                        std::string default_value) const noexcept
{
    return getString(sectionName, name, std::move(default_value));
}

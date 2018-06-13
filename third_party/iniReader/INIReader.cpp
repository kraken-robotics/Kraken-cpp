// Read an INI file into easy-to-access name/value pairs.

#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "INIReader.h"

#if DEBUG
#include <iostream>

#endif


#if USE_FILESYSTEM
#include <cstdio>
#include <cstdlib>
#endif

#if USE_FILESYSTEM
void INIReader::loadFromFile(const std::string& filename)
{
    parseFile(filename);
}
#endif

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

void INIReader::parseNewValue(const std::string &section, const std::string &line)
{
    auto separatorPosition = line.find(':');
    if(separatorPosition == std::string::npos)
    {
        separatorPosition = line.find('=');
        if(separatorPosition == std::string::npos)
        {
            throw (std::invalid_argument("Malformed INI. Incorrect separator in key:value pair at line " + line));
        }
    }

    auto key = makeKey(section, line.substr(0, separatorPosition));
    auto value = line.substr(separatorPosition + 1, line.length());
    values_.emplace(key, value);
}

std::string INIReader::parseNewSection(const std::string& line)
{
    auto endPosition = line.find(']') - 1;
    if(endPosition == std::string::npos)
    {
        throw (std::invalid_argument("Malformed INI. Couldn't find end section delimiter ']' at line " + line));
    }
    return line.substr(1, endPosition);
}

void INIReader::parseFile(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file)
    {
        throw std::invalid_argument("Could not open configuration file.");
    }

    //Get file size
    fseek (file , 0 , SEEK_END);
    auto fileSize = (size_t)ftell (file);
    rewind (file);

    // allocate memory to contain the whole file
    auto buffer = new char[fileSize];
    fread(buffer,1, (long)fileSize,file);

    //Store the content of the buffer in a string and delete it before calling parseString.
    std::string fileContent = std::string(buffer);
    delete(buffer);
    loadFromString(fileContent);
}

void INIReader::loadFromString(std::string fileContent)
{
    std::string currentSection;
    size_t pos;

    while((pos = fileContent.find('\n')) != std::string::npos)
    {
        auto line = fileContent.substr(0, pos);
        fileContent.erase(0, (int)pos + 1);

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
            currentSection = parseNewSection(line);
        }
        else
        {
            //register new key:value pair
            parseNewValue(currentSection, line);
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

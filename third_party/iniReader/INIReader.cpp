// Read an INI file into easy-to-access name/value pairs.

#include <algorithm>
#include <cctype>
#include <cstdlib>

#include "ini.h"
#include "INIReader.h"

#if DEBUG
#include <iostream>
#endif

INIReader::INIReader(std::string filename)
{
    _error = ini_parse(filename.c_str(), valueHandler, this);
#if DEBUG
    if(_error == -1)
    {
        std::cerr << "An error happened while loading the ini file." << std::endl;
    }
#endif
}

INIReader::~INIReader()
{
    // Clean up the field sets
    std::map<std::string, std::set<std::string>*>::iterator fieldSetsIt;
    for (fieldSetsIt = _fields.begin(); fieldSetsIt != _fields.end(); ++fieldSetsIt)
        delete fieldSetsIt->second;
}

int INIReader::ParseError()
{
    return _error;
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

std::string INIReader::getString(const std::string &section, const std::string &name, const std::string &default_value)
{
    std::string key = makeKey(section, name);
    return _values.count(key) ? _values[key] : default_value;
}

int INIReader::getInteger(const std::string &section, const std::string &name, int default_value)
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

float INIReader::getReal(const std::string &section, const std::string &name, float default_value)
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

bool INIReader::getBoolean(const std::string &section, const std::string &name, bool default_value)
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

int INIReader::valueHandler(void *user, const char *section, const char *name,
                            const char *value)
{
    auto reader = (INIReader*)user;

    // Add the value to the lookup map
    std::string key = makeKey(section, name);
    if (!reader->_values[key].empty())
        reader->_values[key] += "\n";
    reader->_values[key] += value;

    // Insert the section in the sections set
    reader->_sections.insert(section);

    // Add the value to the values set
    std::string sectionKey = section;
    std::transform(sectionKey.begin(), sectionKey.end(), sectionKey.begin(), ::tolower);

    std::set<std::string>* fieldsSet;
    auto fieldSetIt = reader->_fields.find(sectionKey);
    if(fieldSetIt==reader->_fields.end())
    {
        fieldsSet = new std::set<std::string>();
        reader->_fields.insert ( std::pair<std::string, std::set<std::string>*>(sectionKey,fieldsSet) );
    } else {
        fieldsSet=fieldSetIt->second;
    }
    fieldsSet->insert(name);

    return 1;
}

template<>
int INIReader::get<int>(const std::string& sectionName, const std::string& name, int default_value)
{
    return getInteger(sectionName, name, default_value);
}

template<>
float INIReader::get<float>(const std::string& sectionName, const std::string& name, float default_value)
{
    return getReal(sectionName, name, default_value);
}

template<>
bool INIReader::get<bool>(const std::string& sectionName, const std::string& name, bool default_value)
{
    return getBoolean(sectionName, name, default_value);
}

template<>
std::string INIReader::get<std::string>(const std::string& sectionName, const std::string& name,
                                        std::string default_value)
{
    return getString(sectionName, name, std::move(default_value));
}

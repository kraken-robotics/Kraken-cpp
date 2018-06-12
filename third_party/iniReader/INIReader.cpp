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
    _error = ini_parse(filename.c_str(), ValueHandler, this);
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

std::string INIReader::Get(const std::string& section, const std::string& name, std::string default_value)
{
    std::string key = MakeKey(section, name);
    return _values.count(key) ? _values[key] : default_value;
}

long INIReader::GetInteger(const std::string& section, const std::string& name, long default_value)
{
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(const std::string& section, const std::string& name, double default_value)
{
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(const std::string& section, const std::string& name, bool default_value)
{
    std::string valstr = Get(section, name, "");
    // Convert to lower case to make std::string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

std::set<std::string> INIReader::GetSections() const
{
    return _sections;
}

std::set<std::string> INIReader::GetFields(const std::string& section) const
{
    std::string sectionKey = section;
    std::transform(sectionKey.begin(), sectionKey.end(), sectionKey.begin(), ::tolower);
    auto fieldSetIt = _fields.find(sectionKey);
    if(fieldSetIt==_fields.end())
        return std::set<std::string>();
    return *(fieldSetIt->second);
}

std::string INIReader::MakeKey(std::string section, std::string name)
{
    std::string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    auto reader = (INIReader*)user;

    // Add the value to the lookup map
    std::string key = MakeKey(section, name);
    if (reader->_values[key].empty())
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
long INIReader::get<long>(const std::string& sectionName, const std::string& name, long default_value)
{
    return GetInteger(sectionName, name, default_value);
}

template<>
int INIReader::get<int>(const std::string& sectionName, const std::string& name, int default_value)
{
    return static_cast<int>(get<long>(sectionName, name, default_value));
}

template<>
double INIReader::get<double>(const std::string& sectionName, const std::string& name, double default_value)
{
    return GetReal(sectionName, name, default_value);
}

template<>
bool INIReader::get<bool>(const std::string& sectionName, const std::string& name, bool default_value)
{
    return GetBoolean(sectionName, name, default_value);
}

template<>
std::string INIReader::get<std::string>(const std::string& sectionName, const std::string& name, std::string default_value)
{
    return Get(sectionName, name, std::move(default_value));
}

// Read an INI file into easy-to-access name/value pairs.

// iniReader and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/

#ifndef __INIREADER_H__
#define __INIREADER_H__

#include <map>
#include <set>
#include <string>

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    explicit INIReader(std::string filename);
    ~INIReader();

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int ParseError();

    // Get a string value from INI file, returning default_value if not found.
    std::string getString(const std::string &section, const std::string &name, const std::string &default_value = "");

    // Get an integer (long) value from INI file, returning default_value if
    // not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
    int getInteger(const std::string &section, const std::string &name, int default_value);

    // Get a real (floating point double) value from INI file, returning
    // default_value if not found or not a valid floating point value
    // according to strtod().
    float getReal(const std::string &section, const std::string &name, float default_value);

    // Get a boolean value from INI file, returning default_value if not found or if
    // not a valid true/false value. Valid true values are "true", "yes", "on", "1",
    // and valid false values are "false", "no", "off", "0" (not case sensitive).
    bool getBoolean(const std::string &section, const std::string &name, bool default_value);

    template<class T>
    T get(const std::string& sectionName, const std::string& name, T default_value);

private:
    static void safeToLower(std::string& stringRef) noexcept;

    int _error;
    std::map<std::string, std::string> _values;
    // Because we want to retain the original casing in _fields, but
    // want lookups to be case-insensitive, we need both _fields and _values
    std::set<std::string> _sections;
    std::map<std::string, std::set<std::string>*> _fields;
    static std::string makeKey(const std::string &section, const std::string &name) noexcept;
    static int valueHandler(void *user, const char *section, const char *name,
                            const char *value);
};

#endif  // __INIREADER_H__

#include "configuration_handler.h"

namespace kraken
{
    ConfigurationHandler::ConfigurationHandler(const std::string& filename) :
        ini_reader_{filename},
        modules_(module_count)
    {
    }

    template<>
    int ConfigurationHandler::getDefaultValue<int>(ConfigKey key) {
        return static_cast<int>(default_values_[static_cast<int>(key)].numeric_value);
    };

    template<>
    float ConfigurationHandler::getDefaultValue<float>(ConfigKey key) {
        return default_values_[static_cast<int>(key)].numeric_value;
    };

    template<>
    bool ConfigurationHandler::getDefaultValue<bool>(ConfigKey key) {
        return default_values_[static_cast<int>(key)].boolean_value;
    };

    template<>
    std::string ConfigurationHandler::getDefaultValue<std::string>(ConfigKey key) {
        return default_values_[static_cast<int>(key)].string_value;
    };

    void ConfigurationHandler::registerCallback(ConfigModule module_enum, ConfigurationCallback callback)
    {
        auto module_instance = getModule(module_enum);
        module_instance->registerCallback(std::move(callback));
    }

    void ConfigurationHandler::changeModuleSection(ConfigModule module_enum, std::string new_section)
    {
        auto module_instance = getModule(module_enum);
        module_instance->changeSection(*this, std::move(new_section));
    }

    void ConfigurationHandler::changeModuleSection(std::vector<ConfigModule>&& modules, std::string new_section)
    {
        for(auto module_instance : modules)
        {
            changeModuleSection(module_instance, new_section);
        }
    }

    std::string ConfigurationHandler::getSectionName(ConfigModule module_key)
    {
        return modules_[static_cast<int>(module_key)].getCurrentSection();
    }

    std::string ConfigurationHandler::getKeyName(ConfigKey key)
    {
        return configuration_key_string_values[static_cast<int>(key)];
    }

    ConfigModule ConfigurationHandler::getModuleEnumFromKeyEnum(ConfigKey key) const noexcept
    {
        for (auto pair : modules_limits) {
            if (key < pair.first)  return pair.second;
        }
        // none of the above
        return ConfigModule::Tentacle;
    }

    ConfigurationModule* ConfigurationHandler::getModule(ConfigModule module_enum)
    {
        return &modules_[static_cast<int>(module_enum)];
    }
}
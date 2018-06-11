#include "configuration_handler.h"

namespace kraken
{
    ConfigurationHandler::ConfigurationHandler(const std::string& filename) :
        ini_reader_{filename},
        modules_(module_count),
        default_values_(configuration_key_count)
    {
        setDefaultValues();
    }

    long ConfigurationHandler::getInt(ConfigKey key, ConfigModule module_enum)
    {
        auto defaultValue = static_cast<int>(default_values_[static_cast<int>(key)].numeric_value);
        auto sectionName = getSectionName(module_enum);
        return ini_reader_.GetInteger(sectionName, getKeyName(key), defaultValue);
    }

    double ConfigurationHandler::getDouble(ConfigKey key, ConfigModule module_enum)
    {
        auto defaultValue = default_values_[static_cast<int>(key)].numeric_value;
        auto sectionName = getSectionName(module_enum);
        return ini_reader_.GetReal(sectionName, getKeyName(key), defaultValue);
    }

    bool ConfigurationHandler::getBool(ConfigKey key, ConfigModule module_enum)
    {
        auto defaultValue = default_values_[static_cast<int>(key)].boolean_value;
        auto sectionName = getSectionName(module_enum);
        return ini_reader_.GetBoolean(sectionName, getKeyName(key), defaultValue);
    }

    std::string ConfigurationHandler::getString(ConfigKey key, ConfigModule module_enum)
    {
        auto defaultValue = default_values_[static_cast<int>(key)].string_value;
        auto sectionName = getSectionName(module_enum);
        return ini_reader_.Get(sectionName, getKeyName(key), defaultValue);
    }

    long ConfigurationHandler::getInt(ConfigKey key)
    {
        return getInt(key, getModuleEnumFromKeyEnum(key));
    }

    double ConfigurationHandler::getDouble(ConfigKey key)
    {
        return getDouble(key, getModuleEnumFromKeyEnum(key));
    }

    bool ConfigurationHandler::getBool(ConfigKey key)
    {
        return getBool(key, getModuleEnumFromKeyEnum(key));
    }

    std::string ConfigurationHandler::getString(ConfigKey key)
    {
        return getString(key, getModuleEnumFromKeyEnum(key));
    }

    void ConfigurationHandler::registerCallback(ConfigModule module_enum, ConfigurationCallback callback)
    {
        auto module_instance = getModule(module_enum);
        if(module_instance)
        {
            module_instance->registerCallback(std::move(callback));
        }
    }

    void ConfigurationHandler::changeModuleSection(ConfigModule module_enum, std::string new_section)
    {
        auto module_instance = getModule(module_enum);
        if(module_instance)
        {
            module_instance->changeSection(*this, std::move(new_section));
        }
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
        //I'm not proud of this function, but I could'nt find a better solution yet.
        if(key < ConfigKey::NecessaryMargin)
        {
            return ConfigModule::Navmesh;
        }
        else if(key < ConfigKey::MaxCurvatureDerivative)
        {
            return ConfigModule::Autoreplanning;
        }
        else if(key < ConfigKey::NodeMemoryPoolSize)
        {
            return ConfigModule::ResearchMechanical;
        }
        else if(key < ConfigKey::PrecisionTrace)
        {
            return ConfigModule::Memory;
        }
        else
        {
            return ConfigModule::Tentacle;
        }
    }

    ConfigurationModule* ConfigurationHandler::getModule(ConfigModule module_enum)
    {
        auto moduleId = static_cast<unsigned int>(module_enum);
        if(moduleId < module_count)
        {
            return &modules_[moduleId];
        }
        else
        {
            return nullptr;
        }
    }

    void ConfigurationHandler::setDefaultValues()
    {
        doAddDefaultValue<int>(ConfigKey::NavmeshObstaclesDilatation, 100);
        doAddDefaultValue<int>(ConfigKey::LargestTriangleAreaInNavmesh, 20000);
        doAddDefaultValue<int>(ConfigKey::LongestEdgeInNavmesh, 200);
        doAddDefaultValue<std::string>(ConfigKey::NavmeshFilename, "navmesh.krk");
        doAddDefaultValue<int>(ConfigKey::NecessaryMargin, 40);
        doAddDefaultValue<int>(ConfigKey::PreferedMargin, 60);
        doAddDefaultValue<int>(ConfigKey::MarginBeforeCollision, 100);
        doAddDefaultValue<int>(ConfigKey::InitialMargin, 100);
        doAddDefaultValue<int>(ConfigKey::MaxCurvatureDerivative, 5);
        doAddDefaultValue<int>(ConfigKey::MaxLateralAcceleration, 3);
        doAddDefaultValue<int>(ConfigKey::MaxLinearAcceleration, 2);
        doAddDefaultValue<int>(ConfigKey::DefaultMaxSpeed, 1);
        doAddDefaultValue<int>(ConfigKey::MinimalSpeed, 0);
        doAddDefaultValue<int>(ConfigKey::MaxCurvature, 5);
        doAddDefaultValue<int>(ConfigKey::StopDuration, 800);
        doAddDefaultValue<int>(ConfigKey::SearchTimeout, 10000);
        doAddDefaultValue<int>(ConfigKey::ThreadNumber, 1);
        doAddDefaultValue<bool>(ConfigKey::EnableDebug, true);
        doAddDefaultValue<bool>(ConfigKey::FastAndDirty, false);
        doAddDefaultValue<bool>(ConfigKey::CheckNewObstacles, false);
        doAddDefaultValue<int>(ConfigKey::NodeMemoryPoolSize, 20000);
        doAddDefaultValue<int>(ConfigKey::ObstaclesMemoryPoolSize, 50000);
        doAddDefaultValue<bool>(ConfigKey::AllowBackwardMotion, true);
        doAddDefaultValue<int>(ConfigKey::NbPoints, 5);
        doAddDefaultValue<float>(ConfigKey::PrecisionTrace, 0.02f);
    }
}
#ifndef CONFIGURATION_HANDLER_H
#define CONFIGURATION_HANDLER_H

#include <string>
#include <functional>

#include "configuration_module.h"
#include "iniReader/INIReader.h"

#include <iostream>

namespace kraken {
    /*
     * Before modifying this enum, consider that:
     * - The enumeration values are used as keys in a vector, they need to be a contiguous range of integers.
     * - NbPoints is used to initialize the size of said vector.
     * - The enumerations are ordered by module (see ConfigModule). If you have to modify this enum, keep the groups in the
     * same order and let the first key of each group at the first place. If you don't do this, update the array
     * modules_limits according to your changes.
     * - The default_values_ initializer list directly depends on the order of the enumerations values.
     */
    namespace ConfigKeys {
        enum class ConfigKeys {
            //Navmesh parameters
            NavmeshObstaclesDilatation = 0,
            LargestTriangleAreaInNavmesh,
            LongestEdgeInNavmesh,
            NavmeshFilename,

            //Auto replanning
            NecessaryMargin,
            PreferedMargin,
            MarginBeforeCollision,
            InitialMargin,

            //Research and mechanical parameters
            MaxCurvatureDerivative,
            MaxLateralAcceleration,
            MaxLinearAcceleration,
            DefaultMaxSpeed,
            MinimalSpeed,
            MaxCurvature,
            StopDuration,
            SearchTimeout,
            ThreadNumber,
            EnableDebug,
            FastAndDirty,
            CheckNewObstacles,
            AllowBackwardMotion,

            //Memory management parameters
            NodeMemoryPoolSize,
            ObstaclesMemoryPoolSize,

            //Tentacle parameters
            PrecisionTrace,
            NbPoints
        };
    }
    using ConfigKey = ConfigKeys::ConfigKeys;

    /*
     * Before modifying this enum, consider that:
     * - The enumeration values are used as keys in a vector, they need to be a contiguous range of integers.
     * - Tentacle is used to initialize the size of said vector.
     */
    namespace ConfigModules {
        enum class ConfigModules {
            Navmesh = 0,        //Require to regenerate the navmesh
            Autoreplanning,     //Can be modified on-the-fly
            ResearchMechanical, //Can be modified on-the-fly
            Memory,             //Require to recreate the pools
            Tentacle
        };
    }
    using ConfigModule = ConfigModules::ConfigModules;

    class ConfigurationHandler {
    private:
        //Structure holding all possible types of parameter value.
        //It should be an union, but it will require a bit more work because of the std::string
        struct ConfigurationParameter {
            float numeric_value = 0;
            bool boolean_value = false;
            std::string string_value = {};

            ConfigurationParameter() = default;

            explicit ConfigurationParameter(float value) { numeric_value = value; }

            explicit ConfigurationParameter(int value) { numeric_value = value; }

            explicit ConfigurationParameter(bool value) { boolean_value = value; }

            explicit ConfigurationParameter(const std::string& value) { string_value = value; }
        };

    public:
        ConfigurationHandler();

#if USE_FILESYSTEM
        void loadFromFile(const std::string& filename);
#endif
        void loadFromString(const std::string& fileContent);

        void registerCallback(ConfigModule module_enum, ConfigurationCallback callback);

        void changeModuleSection(ConfigModule module_enum, std::string new_section);

        void changeModuleSection(std::vector<ConfigModule> &&modules, std::string new_section);

        template<typename T>
        T get(ConfigKey key, ConfigModule module_enum)
        {
            auto defaultValue = getDefaultValue<T>(key);
            auto sectionName = getSectionName(module_enum);
            return ini_reader_.get<T>(sectionName, getKeyName(key), defaultValue);
        }

        template<typename T>
        T get(ConfigKey key)
        {
            return get<T>(key, getModuleEnumFromKeyEnum(key));
        }

    private:
        ConfigModule getModuleEnumFromKeyEnum(ConfigKey key) const noexcept;

        std::string getSectionName(ConfigModule module_key);

        std::string getKeyName(ConfigKey key);

        ConfigurationModule* getModule(ConfigModule module_enum);

        void callCallbacks();

        template<class T>
        T getDefaultValue(ConfigKey key);

        INIReader ini_reader_;
        std::vector<ConfigurationModule> modules_;
        static constexpr unsigned long configuration_key_count = (unsigned long) ConfigKey::NbPoints + 1;
        static constexpr unsigned long module_count = (unsigned long) ConfigModule::Tentacle + 1;

        //This array need to be initialized in the same order as the ConfigKey enum
        const ConfigurationParameter default_values_[configuration_key_count] = {
                ConfigurationParameter{100},                        //NavmeshObstaclesDilatation
                ConfigurationParameter{20000},                      //LargestTriangleAreaInNavmesh
                ConfigurationParameter{200},                        //LongestEdgeInNavmesh
                ConfigurationParameter{(std::string)"navmesh.krk"}, //NavmeshFilename
                ConfigurationParameter{40},                         //NecessaryMargin
                ConfigurationParameter{60},                         //PreferedMargin
                ConfigurationParameter{100},                        //MarginBeforeCollision
                ConfigurationParameter{100},                        //InitialMargin
                ConfigurationParameter{5},                          //MaxCurvatureDerivative
                ConfigurationParameter{3},                          //MaxLateralAcceleration
                ConfigurationParameter{2},                          //MaxLinearAcceleration
                ConfigurationParameter{1},                          //DefaultMaxSpeed
                ConfigurationParameter{0},                          //MinimalSpeed
                ConfigurationParameter{5},                          //MaxCurvature
                ConfigurationParameter{800},                        //StopDuration
                ConfigurationParameter{10000},                      //SearchTimeout
                ConfigurationParameter{1},                          //ThreadNumber
                ConfigurationParameter{true},                       //EnableDebug
                ConfigurationParameter{false},                      //FastAndDirty
                ConfigurationParameter{false},                      //CheckNewObstacles
                ConfigurationParameter{20000},                      //NodeMemoryPoolSize
                ConfigurationParameter{50000},                      //ObstaclesMemoryPoolSize
                ConfigurationParameter{true},                       //AllowBackwardMotion
                ConfigurationParameter{0.02f},                      //PrecisionTrace
                ConfigurationParameter{5}                          //NbPoints
        };

        //The array that keeps the string values of the ConfigKeys
        const std::string configuration_key_string_values[configuration_key_count] = {
                "NavmeshObstaclesDilatation", "LargestTriangleAreaInNavmesh", "LongestEdgeInNavmesh", "NavmeshFilename",
                "NecessaryMargin", "PreferedMargin", "MarginBeforeCollision", "InitialMargin", "MaxCurvatureDerivative",
                "MaxLateralAcceleration", "MaxLinearAcceleration", "DefaultMaxSpeed", "MinimalSpeed", "MaxCurvature",
                "StopDuration", "SearchTimeout", "ThreadNumber", "EnableDebug", "FastAndDirty", "CheckNewObstacles",
                "AllowBackwardMotion", "NodeMemoryPoolSize", "ObstaclesMemoryPoolSize", "PrecisionTrace", "NbPoints"
        };

        const std::pair<ConfigKey, ConfigModule> modules_limits[4] = {
            { ConfigKey::NecessaryMargin, ConfigModule::Navmesh },
            { ConfigKey::MaxCurvatureDerivative, ConfigModule::Autoreplanning },
            { ConfigKey::NodeMemoryPoolSize, ConfigModule::ResearchMechanical },
            { ConfigKey::PrecisionTrace, ConfigModule::Memory }
        };
    };
}
#endif //CONFIGURATION_HANDLER_H

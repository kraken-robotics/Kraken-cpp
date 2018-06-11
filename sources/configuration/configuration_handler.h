#ifndef CONFIGURATION_HANDLER_H
#define CONFIGURATION_HANDLER_H

#include <string>
#include <functional>

#include "configuration_module.h"
#include "iniReader/INIReader.h"

namespace kraken {
    /*
     * Before modifying this enum, consider that:
     * - The enumeration values are used as keys in a vector, they need to be a contiguous range of integers.
     * - NbPoints is used to initialize the size of said vector.
     * - The enumerations are ordered by module (see ConfigModule). If you have to modify this enum, keep the groups in the
     * same order and let the first key of each group at the first place. If you don't do this, update the function
     * getModuleEnumFromKeyEnum according to your changes.
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
            double numeric_value = 0;
            bool boolean_value = false;
            std::string string_value = {};

            ConfigurationParameter() = default;

            explicit ConfigurationParameter(double value) { numeric_value = value; }

            explicit ConfigurationParameter(int value) { numeric_value = value; }

            explicit ConfigurationParameter(bool value) { boolean_value = value; }

            explicit ConfigurationParameter(std::string value) { string_value = value; }
        };

    public:
        explicit ConfigurationHandler(const std::string &filename);

        void registerCallback(ConfigModule module_enum, ConfigurationCallback callback);

        void changeModuleSection(ConfigModule module_enum, std::string new_section);

        void changeModuleSection(std::vector<ConfigModule> &&modules, std::string new_section);

        long getInt(ConfigKey key, ConfigModule module_enum);

        double getDouble(ConfigKey key, ConfigModule module_enum);

        bool getBool(ConfigKey key, ConfigModule module_enum);

        std::string getString(ConfigKey key, ConfigModule module_enum);

        long getInt(ConfigKey key);

        double getDouble(ConfigKey key);

        bool getBool(ConfigKey key);

        std::string getString(ConfigKey key);

    private:
        ConfigModule getModuleEnumFromKeyEnum(ConfigKey key) const noexcept;

        std::string getSectionName(ConfigModule module_key);

        void setDefaultValues();

        std::string getKeyName(ConfigKey key);

        ConfigurationModule* getModule(ConfigModule module_enum);

        //Helper function for default values initialization
        template<typename T>
        void doAddDefaultValue(ConfigKey key, T value) {
            default_values_[(int) key] = ConfigurationParameter{value};
        }

        INIReader ini_reader_;
        std::vector<ConfigurationModule> modules_;
        std::vector<ConfigurationParameter> default_values_;
        static constexpr unsigned long configuration_key_count = (unsigned long) ConfigKey::NbPoints + 1;
        static constexpr unsigned long module_count = (unsigned long) ConfigModule::Tentacle + 1;

        //The array that keeps the string values of the ConfigKeys
        const std::string configuration_key_string_values[configuration_key_count] = {
                "NavmeshObstaclesDilatation", "LargestTriangleAreaInNavmesh", "LongestEdgeInNavmesh", "NavmeshFilename",
                "NecessaryMargin", "PreferedMargin", "MarginBeforeCollision", "InitialMargin", "MaxCurvatureDerivative",
                "MaxLateralAcceleration", "MaxLinearAcceleration", "DefaultMaxSpeed", "MinimalSpeed", "MaxCurvature",
                "StopDuration", "SearchTimeout", "ThreadNumber", "EnableDebug", "FastAndDirty", "CheckNewObstacles",
                "AllowBackwardMotion", "NodeMemoryPoolSize", "ObstaclesMemoryPoolSize", "PrecisionTrace", "NbPoints"
        };
    };
}
#endif //CONFIGURATION_HANDLER_H

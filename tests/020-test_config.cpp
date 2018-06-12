#include "catch/catch.hpp"
#include <iostream>
#include "../sources/configuration/configuration_handler.h"

TEST_CASE("Configuration", "[Configuration]")
{
    using kraken::ConfigurationHandler;
    using kraken::ConfigModule;
    using kraken::ConfigKey;
    ConfigurationHandler handler{"../tests/test.ini"};

    //Test default section load
    REQUIRE (handler.get<int>(ConfigKey::LongestEdgeInNavmesh) == 9);
    REQUIRE (handler.get<int>(ConfigKey::LongestEdgeInNavmesh, ConfigModule::Navmesh) == 9);
    REQUIRE (handler.get<bool>(ConfigKey::EnableDebug));
    REQUIRE (handler.get<bool>(ConfigKey::EnableDebug, ConfigModule::ResearchMechanical));

    //Test hard coded default values
    REQUIRE(handler.get<int>(ConfigKey::NavmeshObstaclesDilatation) == 100);
    REQUIRE(handler.get<std::string>(ConfigKey::NavmeshFilename) == "navmesh.krk");
    REQUIRE(handler.get<std::string>(ConfigKey::NavmeshFilename, ConfigModule::Navmesh) == "navmesh.krk");
    REQUIRE(handler.get<double>(ConfigKey::PrecisionTrace) == 0.02f);

    //Register a function to be called when the configuration changes for Navmesh module
    handler.registerCallback(ConfigModule::Navmesh, [] (ConfigurationHandler& ch) {
        static int passCount = 0;
        if(passCount == 0)
        {
            //Test section change on module Navmesh
            REQUIRE (ch.get<int>(ConfigKey::LongestEdgeInNavmesh) == 1);
            REQUIRE (ch.get<bool>(ConfigKey::EnableDebug));
        }
        else if(passCount == 1)
        {
            //Test section change on both modules.
            //Navmesh is first in the changeSection call, so EnableDebug is false.
            REQUIRE (ch.get<int>(ConfigKey::LongestEdgeInNavmesh) == 2);
            REQUIRE (!ch.get<bool>(ConfigKey::EnableDebug));
        }
        passCount++;
    });

    //Register a function to be called when the configuration changes for ResearchMechanical module
    handler.registerCallback(ConfigModule::ResearchMechanical, [] (ConfigurationHandler& ch) {
        static int passCount = 0;
        if(passCount == 0)
        {
            //Test section change on module ResearchMechanical
            REQUIRE (ch.get<int>(ConfigKey::LongestEdgeInNavmesh) == 1);
            REQUIRE (!ch.get<bool>(ConfigKey::EnableDebug));
        }
        else if(passCount == 1)
        {
            //Test section change on both modules.
            //Navmesh is first in the changeSection call, so LongestEdgeInNavmesh == 2.
            REQUIRE (ch.get<int>(ConfigKey::LongestEdgeInNavmesh) == 2);
            REQUIRE (ch.get<bool>(ConfigKey::EnableDebug));
        }
        passCount++;
    });

    handler.changeModuleSection(ConfigModule::Navmesh, "test1");
    handler.changeModuleSection(ConfigModule::ResearchMechanical, "test1");
    handler.changeModuleSection({ConfigModule::Navmesh, ConfigModule::ResearchMechanical}, "test2");
}
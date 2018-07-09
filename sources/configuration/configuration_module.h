#ifndef CONFIGURATION_MODULE_H
#define CONFIGURATION_MODULE_H

#include <string>

#include "configuration_callback_holder.h"


namespace kraken
{
    class ConfigurationHandler;

    class ConfigurationModule
    {
    public:
        void registerCallback(ConfigurationCallback callback);

        void changeSection(ConfigurationHandler &configuration_handler, std::string new_section);

        std::string getCurrentSection();

        void callCallbacks(ConfigurationHandler &configuration_handler) const;

    private:
        ConfigurationCallbackHolder callbacks_holder_;
        std::string current_section_ = {"default"};
    };
}

#endif //CONFIGURATION_MODULE_H

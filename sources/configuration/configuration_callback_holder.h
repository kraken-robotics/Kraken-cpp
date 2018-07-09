#ifndef CONFIGURATION_CALLBACK_HOLDER_H
#define CONFIGURATION_CALLBACK_HOLDER_H

#include <vector>
#include <string>
#include <functional>


namespace kraken
{
    class ConfigurationHandler;

    using ConfigurationCallback = std::function<void(ConfigurationHandler &)>;

    class ConfigurationCallbackHolder
    {
    public:
        void operator+=(const ConfigurationCallback callback);

        void operator()(ConfigurationHandler &configuration_handler) const;

    private:
        std::vector<ConfigurationCallback> callbacks_;
    };
}

#endif //CONFIGURATION_CALLBACK_HOLDER_H

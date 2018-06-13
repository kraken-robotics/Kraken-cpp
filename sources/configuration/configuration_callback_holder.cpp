#include "configuration_callback_holder.h"

namespace kraken
{
    void ConfigurationCallbackHolder::operator+=(const ConfigurationCallback callback)
    {
        callbacks_.push_back(callback);
    }

    void ConfigurationCallbackHolder::operator()(ConfigurationHandler &configuration_handler) const
    {
        for (const auto& iterator : callbacks_) {
            (iterator)(configuration_handler);
        }
    }
}
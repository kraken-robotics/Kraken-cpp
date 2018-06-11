#include "configuration_callback_holder.h"

namespace kraken
{
    void ConfigurationCallbackHolder::operator+=(const ConfigurationCallback callback)
    {
        callbacks_.push_back(callback);
    }

    void ConfigurationCallbackHolder::operator()(ConfigurationHandler &configuration_handler) const
    {
        auto iterator = callbacks_.cbegin();
        for (; iterator != callbacks_.cend(); ++iterator) {
            (*iterator)(configuration_handler);
        }
    }
}
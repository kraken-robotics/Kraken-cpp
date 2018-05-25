#include "math_utils.h"
#include <cmath>

namespace kraken
{
    float math_utils::computeNewOrientation(const float &orientation)
    {
        float computed_orientation = std::fmod(orientation, 2.f * static_cast<float>(M_PI));
        if (computed_orientation < 0)
            computed_orientation += 2.f * static_cast<float>(M_PI);

        return computed_orientation;
    }

    float math_utils::angleDifference(const float &angle_1, const float &angle_2)
    {
        float deltaO = std::fmod(angle_1 - angle_2, 2.f * static_cast<float >(M_PI));
        if (deltaO > static_cast<float >(M_PI))
            deltaO -= 2.f * static_cast<float>(M_PI);
        else if (deltaO < -static_cast<float >(M_PI))
            deltaO += 2.f * static_cast<float>(M_PI);
        return deltaO;
    }
}
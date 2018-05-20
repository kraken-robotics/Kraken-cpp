#include "oriented_position_2d.h"
#include <cmath>
#include <algorithm>

namespace kraken
{
    OrientedPosition2D::OrientedPosition2D(const float &x, const float &y) : x_(x), y_(y), orientation_(0)
    {

    }

    OrientedPosition2D::OrientedPosition2D(const float &x, const float &y, const float &orientation) : x_(x), y_(y),
                                                                                  orientation_(orientation)
    {

    }

    float OrientedPosition2D::angle_difference(const float &angle_1, const float &angle_2)
    {
        float deltaO = std::fmod(angle_1 - angle_2, 2.f * static_cast<float >(M_PI));
        if (deltaO > M_PI)
            deltaO -= 2 * M_PI;
        else if (deltaO < -M_PI)
            deltaO += 2 * M_PI;
        return deltaO;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const kraken::OrientedPosition2D &v)
    {
        return strm << "OrientedPosition2D(" << v.x_ << "," << v.y_ << "," << v.orientation_ << ")" << std::endl;
    }

#endif
}
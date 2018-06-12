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

    float OrientedPosition2D::getX() const
    {
        return x_;
    }

    float OrientedPosition2D::getY() const
    {
        return y_;
    }

    float OrientedPosition2D::getOrientation() const
    {
        return orientation_;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const kraken::OrientedPosition2D &v)
    {
        return strm << "OrientedPosition2D(" << v.x_ << "," << v.y_ << "," << v.orientation_ << ")" << std::endl;
    }

#endif
}
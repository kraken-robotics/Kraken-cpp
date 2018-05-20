#include "kinematic.h"

#include <cmath>

namespace kraken
{

    Kinematic::Kinematic()
            : Vector2D(0, 0), geometric_orientation_(0), geometric_curvature_(0), real_orientation_(0),
              real_curvature_(0), go_forward_(true), stop_(false)
    {

    }

    Kinematic::Kinematic(const float &x, const float &y, const float &orientation)
    {
        go_forward_ = true;
        stop_ = false;
        update_real(x, y, orientation, 0);
    }

    Kinematic::Kinematic(const float &x, const float &y, const float &orientation, const bool &go_forward,
                         const float &curvature, const bool &stop)
    {
        update(x, y, orientation, go_forward, curvature, stop);
    }

    void Kinematic::update(const ItineraryPoint &iP)
    {
        go_forward_ = iP.getGoingForward();
        update_real(iP.getX(), iP.getY(), iP.getOrientation(), iP.getCurvature());
        stop_ = iP.getStop();
    }

    void Kinematic::update_real(const float &x, const float &y, const float &real_orientation,
                                const float &real_curvature)
    {
        if (go_forward_)
        {
            geometric_orientation_ = real_orientation;
            geometric_curvature_ = real_curvature;
        } else
        {
            geometric_orientation_ = real_orientation + static_cast<float>(M_PI);
            geometric_curvature_ = -real_curvature;
        }

        x_ = x;
        y_ = y;
        real_orientation_ = real_orientation;
        real_curvature_ = real_curvature;
    }

    void Kinematic::update(const float &x, const float &y, const float &geometric_orientation, const bool &go_forward,
                           const float &geometric_curvature, const bool &stop)
    {
        if (go_forward)
        {
            real_orientation_ = geometric_orientation;
            real_curvature_ = geometric_curvature;
        } else
        {
            real_orientation_ = geometric_orientation + static_cast<float>(M_PI);
            real_curvature_ = -geometric_curvature;
        }

        x_ = x;
        y_ = y;
        geometric_orientation_ = geometric_orientation;
        geometric_curvature_ = geometric_curvature;
        go_forward_ = go_forward;
        stop_ = stop;
    }
}

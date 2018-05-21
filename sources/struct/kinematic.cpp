#include "kinematic.h"

#include <cmath>

namespace kraken
{

    Kinematic::Kinematic()
            : position_(0, 0), geometric_orientation_(0), geometric_curvature_(0), real_orientation_(0),
              real_curvature_(0), go_forward_(true), stop_(false)
    {

    }

    Kinematic::Kinematic(const float &x, const float &y, const float &orientation)
            : position_(x, y), geometric_orientation_(0), geometric_curvature_(0), real_orientation_(orientation),
              real_curvature_(0), go_forward_(true), stop_(false)
    {
        updateReal(x, y, orientation, 0);
    }

    Kinematic::Kinematic(const float &x, const float &y, const float &orientation, const bool &go_forward,
                         const float &curvature, const bool &stop)
            : position_(x, y), geometric_orientation_(0), geometric_curvature_(0), real_orientation_(orientation),
              real_curvature_(0), go_forward_(true), stop_(false)
    {
        update(x, y, orientation, go_forward, curvature, stop);
    }

    bool Kinematic::isSimilar(const Kinematic &rhs, const float &squaredDeltaPos,
                              const float &deltaCurvature, const float &deltaOrientation) const
    {
        return rhs.position_.squaredDistance(position_) < squaredDeltaPos
               && std::abs(real_curvature_ - rhs.real_curvature_) < deltaCurvature
               && std::abs(real_orientation_ - rhs.real_orientation_) < deltaOrientation
               // TODO : use angle_difference
               && rhs.go_forward_ == go_forward_ && rhs.stop_ == stop_;
    }

    void Kinematic::update(const ItineraryPoint &iP)
    {
        go_forward_ = iP.getGoingForward();
        updateReal(iP.getX(), iP.getY(), iP.getOrientation(), iP.getCurvature());
        stop_ = iP.getStop();
    }

    void Kinematic::updateReal(const float &x, const float &y, const float &real_orientation,
                               const float &real_curvature)
    {
        if (go_forward_)
        {
            geometric_orientation_ = real_orientation;
            geometric_curvature_ = real_curvature;
        }
        else
        {
            geometric_orientation_ = real_orientation + static_cast<float>(M_PI);
            geometric_curvature_ = -real_curvature;
        }

        position_.setX(x);
        position_.setX(y);
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
        }
        else
        {
            real_orientation_ = geometric_orientation + static_cast<float>(M_PI);
            real_curvature_ = -geometric_curvature;
        }

        position_.setX(x);
        position_.setX(y);
        geometric_orientation_ = geometric_orientation;
        geometric_curvature_ = geometric_curvature;
        go_forward_ = go_forward;
        stop_ = stop;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const kraken::Kinematic &v)
    {
        return strm << "Kinematic(" << v.position_.getX() << ", " << v.position_.getY() << ", orientation :"
                    << v.real_orientation_ << "," << (v.go_forward_ ? "going forward" : "going backward")
                    << ", curvate :" << v.real_curvature_ << (v.stop_ ? ")" : " stop)") << std::endl;
    }

#endif
}

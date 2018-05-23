#include "itinerary_point.h"
#include <cmath>

namespace kraken
{

    ItineraryPoint::ItineraryPoint(const Vector2D &pos,
                                   const float &orientation, const float &curvature,
                                   const bool &going_forward, const float &max_speed,
                                   const float &possible_speed, const bool &stop) :
            pos_(pos), orientation_(computeNewOrientation(orientation)), curvature_(curvature), max_speed_(max_speed),
            possible_speed_(possible_speed), going_forward_(going_forward), stop_(stop)
    {

    }

    bool ItineraryPoint::operator==(const ItineraryPoint &rhs) const
    {
        return pos_ == rhs.pos_ && orientation_ == rhs.orientation_
               && curvature_ == rhs.curvature_ && going_forward_ == rhs.going_forward_
               && max_speed_ == rhs.max_speed_ && possible_speed_ == rhs.possible_speed_
               && stop_ == rhs.stop_;
    }

    float ItineraryPoint::computeNewOrientation(const float &orientation)
    {
        float computed_orientation = std::fmod(orientation, 2.f * static_cast<float>(M_PI));
        if (computed_orientation < 0)
            computed_orientation += 2.f * static_cast<float>(M_PI);

        return computed_orientation;
    }

    float ItineraryPoint::getX() const
    {
        return pos_.getX();
    }

    float ItineraryPoint::getY() const
    {
        return pos_.getY();
    }

    float ItineraryPoint::getOrientation() const
    {
        return orientation_;
    }

    float ItineraryPoint::getCurvature() const
    {
        return curvature_;
    }

    bool ItineraryPoint::getGoingForward() const
    {
        return going_forward_;
    }

    bool ItineraryPoint::getStop() const
    {
        return stop_;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const ItineraryPoint &i)
    {
        return strm << "ItineraryPoint(" << i.pos_ << ", o : " << i.orientation_
                    << (i.going_forward_ ? ", going forward" : ", going backward")
                    << ", c : " << i.curvature_ << ", max speed : " << i.max_speed_
                    << ", possible speed : " << i.possible_speed_ << (i.stop_ ? ", ending with a stop)" : ")");
    }

#endif

}

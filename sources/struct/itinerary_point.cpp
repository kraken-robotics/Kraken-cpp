#include "itinerary_point.h"
#include <math.h>

namespace kraken
{

    ItineraryPoint::ItineraryPoint(const float &x, const float &y,
            const float &orientation, const float &curvature,
            const bool &going_forward, const float &max_speed,
            const float &possible_speed, const bool &stop) :
        x_(x), y_(y),
        orientation_(orientation), curvature_(curvature),
        going_forward_(going_forward), max_speed_(max_speed),
        possible_speed_(possible_speed), stop_(stop)
    {
        orientation_ = fmod(orientation_, 2 * M_PI);
        if(orientation_ < 0)
            orientation_ += 2 * M_PI;
    }


    bool ItineraryPoint::operator==(const ItineraryPoint &rhs) const
    {
        return x_ == rhs.x_ && y_ == rhs.y_ && orientation_ == rhs.orientation_
            && curvature_ == rhs.curvature_ && going_forward_ == rhs.going_forward_
            && max_speed_ == rhs.max_speed_ && possible_speed_ == rhs.possible_speed_
            && stop_ == rhs.stop_;
    }

    std::ostream &operator<<(std::ostream &strm, const ItineraryPoint &i)
    {
        return strm << "ItineraryPoint(" << i.x_ << ", " << i.y_ << ", o : " << i.orientation_
            << (i.going_forward_ ? ", going forward" : ", going backward")
            << ", c : " << i.curvature_ << ", max speed : " << i.max_speed_
            << ", possible speed : " << i.possible_speed_ << (i.stop_ ? ", ending with a stop)" : ")");
    }

}

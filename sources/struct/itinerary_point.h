#ifndef KRAKEN_ITINERARYPOINT_H
#define KRAKEN_ITINERARYPOINT_H

#include <ostream>

namespace kraken
{

    class ItineraryPoint
    {
    public:
        ItineraryPoint(const float &x, const float &y,
            const float &orientation, const float &curvature,
            const bool &going_forward, const float &max_speed,
            const float &possible_speed, const bool &stop);

        bool operator==(const ItineraryPoint &rhs) const;

    private:
        float x_;
        float y_;
        float orientation_;
        float curvature_;
        float max_speed_;
        float possible_speed_;
        bool going_forward_;
        bool stop_;

        friend std::ostream &operator<<(std::ostream &strm, const ItineraryPoint &v);
    };
}



#endif //KRAKEN_ITINERARYPOINT_H

#ifndef KRAKEN_ITINERARYPOINT_H
#define KRAKEN_ITINERARYPOINT_H

#if DEBUG
#include <ostream>
#endif

#include "vector_2d.h"

namespace kraken
{

    class ItineraryPoint
    {
    public:
        ItineraryPoint(const Vector2D &pos,
            const float &orientation, const float &curvature,
            const bool &going_forward, const float &max_speed,
            const float &possible_speed, const bool &stop);

        bool operator==(const ItineraryPoint &rhs) const;

        float getX() const;
        float getY() const;
        float getOrientation() const;
        float getCurvature() const;
        bool getGoingForward() const;
        bool getStop() const;

    private:
        float computeNewOrientation(const float &orientation);

    private:
        const Vector2D pos_;
        const float orientation_;
        const float curvature_;
        const float max_speed_;
        const float possible_speed_;
        const bool going_forward_;
        const bool stop_;

#if DEBUG
        friend std::ostream &operator<<(std::ostream &strm, const ItineraryPoint &v);
#endif
    };
}



#endif //KRAKEN_ITINERARYPOINT_H

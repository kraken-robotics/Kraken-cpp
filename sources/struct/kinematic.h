#ifndef TESTS_KINEMATIC_H
#define TESTS_KINEMATIC_H

#include "vector_2d.h"
#include "itinerary_point.h"

namespace kraken
{
    class Kinematic
    {
    public:
        Kinematic();
        Kinematic(const float &x, const float &y, const float &orientation);
        Kinematic(const float &x, const float &y, const float &orientation, const bool &go_forward,
                  const float &curvature, const bool &stop);

        void update(const ItineraryPoint &itineraryPoint);
        void updateReal(const float &x, const float &y, const float &real_orientation, const float &real_curvature);

        bool isSimilar(const Kinematic &rhs, const float &squaredDeltaPos,
                       const float &deltaCurvature, const float &deltaOrientation) const;

    protected:
        void update(const float &x, const float &y, const float &geometric_orientation, const bool &go_forward,
                    const float &geometric_curvature, const bool &stop);
    protected:
        Vector2D position_;
        float geometric_orientation_;
        float geometric_curvature_;

        float real_orientation_;
        float real_curvature_;

        bool go_forward_;
        bool stop_;

#if DEBUG
        friend std::ostream &operator<<(std::ostream &strm, const Kinematic &v);
#endif
    };
}

#endif //TESTS_KINEMATIC_H

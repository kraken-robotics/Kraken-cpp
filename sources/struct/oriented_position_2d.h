#ifndef TESTS_ORIENTED_POSITION_2D_H
#define TESTS_ORIENTED_POSITION_2D_H

#if DEBUG
#include <ostream>
#endif

namespace kraken
{
    class OrientedPosition2D
    {
    public:
        OrientedPosition2D();
        OrientedPosition2D(const float &x, const float &y);
        OrientedPosition2D(const float &x, const float &y, const float &orientation);

        float getX() const;
        float getY() const;
        float getOrientation() const;

    public:
        static float angle_difference(const float &angle_1, const float &angle_2);

    protected:
        float x_;
        float y_;
        float orientation_;

#if DEBUG
        friend std::ostream &operator<<(std::ostream &strm, const OrientedPosition2D &v);
#endif
    };
}

#endif //TESTS_ORIENTED_POSITION_2D_H

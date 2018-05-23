#ifndef TESTS_MATH_UTILS_H
#define TESTS_MATH_UTILS_H

#include "../struct/vector_2d.h"

namespace kraken
{
    namespace math_utils
    {
        float angleDifference(const float &angle_1, const float &angle_2);
        float computeNewOrientation(const float &orientation);
        bool isCircumscribed(const Vector2D &point_a, const Vector2D &point_b, const Vector2D &point_c,
                                    const Vector2D &point_d);
        bool containsNode(const Vector2D &point_a, const Vector2D &point_b,
                                 const Vector2D &point_c);
        float crossProduct(const Vector2D &point_a, const Vector2D &point_b,
                                  const Vector2D &point_c);
        bool isCounterclockwise(const Vector2D &point_a, const Vector2D &point_b,
                                          const Vector2D &point_c);
        bool isInside(const Vector2D &pos, const Vector2D &point_a, const Vector2D &point_b,
                             const Vector2D &point_c);
    }
}

#endif //TESTS_// MATH_UTILS_H

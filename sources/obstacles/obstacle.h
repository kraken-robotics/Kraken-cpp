#ifndef TESTS_OBSTACLE_H
#define TESTS_OBSTACLE_H

#include <vector>
#include "../struct/vector_2d.h"

namespace kraken
{
    class RectangularObstacle;

    class Obstacle
    {
    public:
        explicit Obstacle(const Vector2D &rotation_center) noexcept;
        virtual ~Obstacle() = default;
        virtual bool isInObstacle(const Vector2D &pos) const noexcept = 0;
        virtual float squaredDistance(const Vector2D &pos) const = 0;
        virtual void getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                           std::vector<Vector2D> &vector_2d_list) const = 0;
        virtual bool isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept = 0;
        virtual bool isColliding(const RectangularObstacle &obs) const noexcept = 0;
        virtual bool operator==(const Obstacle &rhs) const noexcept;

        constexpr Vector2D getRotationCenter() const noexcept
        {
            return rotation_center_;
        }
    protected:
        Vector2D rotation_center_;

    };
}

#endif //TESTS_OBSTACLE_H

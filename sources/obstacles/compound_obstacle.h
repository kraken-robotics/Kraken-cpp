#ifndef TESTS_COMPOUND_OBSTACLE_H
#define TESTS_COMPOUND_OBSTACLE_H

#include <vector>
#include "obstacle.h"

namespace kraken
{

    class CompoundObstacle : public Obstacle
    {
    public:
        CompoundObstacle(const Vector2D &rotation_center, std::vector<Obstacle> obstacles_list) noexcept;

        bool isInObstacle(const Vector2D &pos) const noexcept override;
        float squaredDistance(const Vector2D &pos) const override;
        void getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                   std::vector<Vector2D> &vector_2d_list) const override;
        bool isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept override;
        bool isColliding(const RectangularObstacle &obs) const noexcept override;

        bool operator==(const Obstacle &rhs) const noexcept override;

    protected:
        const std::vector<Obstacle> obstacles_list_;
    };

}
#endif //TESTS_COMPOUND_OBSTACLE_H

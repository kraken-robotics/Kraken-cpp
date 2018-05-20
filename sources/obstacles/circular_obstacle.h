#ifndef TESTS_CIRCULAR_OBSTACLE_H
#define TESTS_CIRCULAR_OBSTACLE_H

#include "../struct/vector_2d.h"
#include "obstacle.h"

namespace kraken
{
    class CircularObstacle : public Obstacle
    {
    public:
        CircularObstacle(const Vector2D &pos, float radius);
        ~CircularObstacle() = 0;
        bool isInObstacle(const Vector2D &pos) const;
        float squaredDistance(const Vector2D &pos) const;
        Vector2D* getExpandedConvexHull(float expansion, float longestAllowedLength) const;
        bool isColliding(const Vector2D &point_a, const Vector2D &point_b) const;

    protected:
        float const radius_;
        float const squared_radius_;
    };
}

#endif //TESTS_CIRCULAR_OBSTACLE_H

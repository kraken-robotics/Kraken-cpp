#ifndef TESTS_OBSTACLE_H
#define TESTS_OBSTACLE_H

#include "../struct/vector_2d.h"

namespace kraken
{

    class Obstacle
    {
    public:
        virtual bool isInObstacle(Vector2D pos) = 0;
        virtual float squaredDistance(Vector2D pos) = 0;
        virtual Vector2D* getExpandedConvexHull(float expansion, float longestAllowedLength) = 0;
        virtual bool isColliding(Vector2D point_a, Vector2D point_b) = 0;
        Obstacle(const Vector2D &rotation_center);
        bool operator==(const Obstacle &rhs) const;
        //virtual bool isColliding(RectangularObstacle obs)

    protected:
        Vector2D rotation_center_;

    };
}

#endif //TESTS_OBSTACLE_H

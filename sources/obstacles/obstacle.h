#ifndef TESTS_OBSTACLE_H
#define TESTS_OBSTACLE_H

#include "../struct/vector_2d.h"

namespace kraken
{

    class Obstacle
    {
    public:
        Obstacle(const Vector2D &rotation_center);
        virtual ~Obstacle() = 0;
        virtual bool isInObstacle(const Vector2D &pos) const = 0;
        virtual float squaredDistance(const Vector2D &pos) const = 0;
        virtual Vector2D *getExpandedConvexHull(float expansion, float longestAllowedLength) const = 0;
        virtual bool isColliding(const Vector2D &point_a, const Vector2D &point_b) const = 0;
        bool operator==(const Obstacle &rhs) const;
        //virtual bool isColliding(RectangularObstacle obs)

    protected:
        Vector2D rotation_center_;

    };
}

#endif //TESTS_OBSTACLE_H

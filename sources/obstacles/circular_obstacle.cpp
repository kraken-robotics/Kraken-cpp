#include "obstacle.h"
#include "circular_obstacle.h"
#include "../struct/vector_2d.h"
#include <cmath>

namespace kraken
{
    CircularObstacle::CircularObstacle(const Vector2D &pos, float radius) :
            Obstacle(pos), radius_(radius), squared_radius_(radius * radius)
    {
    }

    bool CircularObstacle::isInObstacle(const Vector2D &pos) const
    {
        return pos.squaredDistance(rotation_center_) <= squared_radius_;
    }

    float CircularObstacle::squaredDistance(const Vector2D &pos) const
    {
        // TODO : is "distance" really necessary ? (it uses a std::sqrt computation)
        float out = std::max(0.f, pos.distance(rotation_center_) - radius_);
        return out * out;
    }

    bool CircularObstacle::isColliding(const Vector2D &point_a, const Vector2D &point_b) const
    {
        const Vector2D point_c = rotation_center_;
        const Vector2D ab = point_b - point_a;
        const Vector2D ac = point_c - point_a;
        float numerator = std::abs(ab.getX() * ac.getY() - ab.getY() * ac.getX());
        float denominator = ab.squaredNorm();
        float distance = numerator * numerator / denominator;

        // no collision with the line (AB)
        if (distance > squared_radius_)
            return false;

        // collision with the segment [AB]
        float pscal1 = ab.dot(ac);
        float pscal2 = -ab.dot(point_c - point_b);
        if (pscal1 >= 0 && pscal2 >= 0)
            return true;

        // check if A or B is in the circle
        return isInObstacle(point_a) || isInObstacle(point_b);
    }

    Vector2D *CircularObstacle::getExpandedConvexHull(float expansion, float longestAllowedLength) const
    {
        int nbPoints = std::ceil(M_2_PI * (radius_ + expansion) / longestAllowedLength);
        if (nbPoints < 3)
            nbPoints = 3;

        Vector2D *hull = new Vector2D[nbPoints];
        for (int i = 0; i < nbPoints; ++i)
            hull[i] = Vector2D::fromPolar(expansion + radius_, i * M_2_PI / nbPoints) + rotation_center_;

        return hull;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const CircularObstacle &o)
    {
        return strm << "CircularObstacle(" << o.rotation_center_ << "," << o.radius_ << ")" << std::endl;
    }

#endif

}
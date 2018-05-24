#include "obstacle.h"
#include "circular_obstacle.h"
#include "rectangular_obstacle.h"
#include "../struct/vector_2d.h"
#include <cmath>
#include <typeinfo>

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

    void CircularObstacle::getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                                 std::vector<Vector2D> &vector_2d_list) const
    {
        int nbPoints = std::ceil(M_2_PI * (radius_ + expansion) / longestAllowedLength);
        if (nbPoints < 3)
            nbPoints = 3;

        for (int i = 0; i < nbPoints; ++i)
            vector_2d_list.push_back(
                    Vector2D::fromPolar(expansion + radius_, i * M_2_PI / nbPoints) + rotation_center_);
    }

    bool CircularObstacle::operator==(const Obstacle &rhs) const
    {
        if (!Obstacle::operator==(rhs))
            return false;

        if (typeid(*this) != typeid(rhs))
            return false;

        return radius_ == static_cast<const CircularObstacle &>(rhs).radius_;
    }

    bool CircularObstacle::isColliding(const RectangularObstacle &obs) const
    {
        if (rotation_center_.squaredDistance(obs.getRotationCenter()) >=
            (radius_ + obs.getHalfDiagonal()) * (radius_ + obs.getHalfDiagonal()))
            return false;
        return obs.squaredDistance(rotation_center_) < squared_radius_;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const CircularObstacle &o)
    {
        return strm << "CircularObstacle(" << o.rotation_center_ << "," << o.radius_ << ")" << std::endl;
    }

#endif

}

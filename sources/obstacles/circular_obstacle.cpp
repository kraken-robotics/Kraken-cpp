#include "obstacle.h"
#include "circular_obstacle.h"
#include "rectangular_obstacle.h"
#include "../struct/vector_2d.h"
#include <cmath>
#include <typeinfo>

namespace kraken
{
    CircularObstacle::CircularObstacle(const Vector2D &pos, float radius) noexcept :
            Obstacle(pos), radius_(radius), squared_radius_(radius * radius)
    {}

    bool CircularObstacle::isInObstacle(const Vector2D &pos) const noexcept
    {
        return pos.squaredDistance(rotation_center_) <= squared_radius_;
    }

    float CircularObstacle::squaredDistance(const Vector2D &pos) const
    {
        // TODO : is "distance" really necessary ? (it uses a std::sqrt computation)
        float out = std::max(0.f, pos.distance(rotation_center_) - radius_);
        return out * out;
    }

    bool CircularObstacle::isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept
    {
        const auto point_c = rotation_center_;
        const auto ab = point_b - point_a;
        const auto ac = point_c - point_a;
        auto numerator = std::abs(ab.getX() * ac.getY() - ab.getY() * ac.getX());
        auto denominator = ab.squaredNorm();
        auto distance = numerator * numerator / denominator;

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
                                                 std::vector<Vector2D> &vector_2d_list) const noexcept
    {
        auto nbPoints = static_cast<int>(std::ceil(M_2_PI * (radius_ + expansion) / longestAllowedLength));
        if (nbPoints < 3)
            nbPoints = 3;

        for (int i = 0; i < nbPoints; ++i)
            vector_2d_list.push_back(
                    Vector2D::fromPolar(expansion + radius_, i * static_cast<float>(M_2_PI) / nbPoints) +
                    rotation_center_);
    }

    bool CircularObstacle::operator==(const Obstacle &rhs) const noexcept
    {
        return Obstacle::operator==(rhs) && typeid(*this) == typeid(rhs) &&
               radius_ == static_cast<const CircularObstacle &>(rhs).radius_;
    }

    bool CircularObstacle::isColliding(const RectangularObstacle &obs) const noexcept
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

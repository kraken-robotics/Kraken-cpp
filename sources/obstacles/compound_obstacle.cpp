#include "compound_obstacle.h"

#include <limits>
#include <typeinfo>

namespace kraken
{
    CompoundObstacle::CompoundObstacle(const Vector2D &rotation_center,
                                       std::vector<Obstacle> obstacles_list) noexcept :
            Obstacle(rotation_center), obstacles_list_(std::move(obstacles_list))
    {
    }

    bool CompoundObstacle::isInObstacle(const Vector2D &pos) const noexcept
    {
        for (const auto &o : obstacles_list_)
        {
            if (o.isInObstacle(pos))
                return true;
        }

        return false;
    }

    float CompoundObstacle::squaredDistance(const Vector2D &pos) const
    {
        float min = std::numeric_limits<float>::max();
        for (const auto &o : obstacles_list_)
        {
            min = std::min(min, o.squaredDistance(pos));
            if (min == 0)
                break;
        }
        return min;
    }

    void CompoundObstacle::getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                                 std::vector<Vector2D> &vector_2d_list) const
    {
        for (const auto &o : obstacles_list_)
            o.getExpandedConvexHull(expansion, longestAllowedLength, vector_2d_list);
    }

    bool CompoundObstacle::isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept
    {
        for (const auto &o : obstacles_list_)
        {
            if (o.isColliding(point_a, point_b))
                return true;
        }
        return false;
    }

    bool CompoundObstacle::isColliding(const RectangularObstacle &obs) const noexcept
    {
        for (const auto &o : obstacles_list_)
        {
            if (o.isColliding(obs))
                return true;
        }
        return false;
    }

    bool CompoundObstacle::operator==(const Obstacle &rhs) const noexcept
    {
        return Obstacle::operator==(rhs) && typeid(*this) == typeid(rhs) &&
               static_cast<const CompoundObstacle &>(rhs).obstacles_list_ == obstacles_list_;
    }
}
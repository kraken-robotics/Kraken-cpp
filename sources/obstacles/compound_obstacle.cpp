#include "compound_obstacle.h"

#include <limits>

kraken::CompoundObstacle::CompoundObstacle(const kraken::Vector2D &rotation_center,
                                           std::vector<kraken::Obstacle> obstacles_list) :
        Obstacle(rotation_center), obstacles_list_(std::move(obstacles_list))
{
}

bool kraken::CompoundObstacle::isInObstacle(const kraken::Vector2D &pos) const
{
    for (auto const &o : obstacles_list_)
    {
        if (o.isInObstacle(pos))
            return true;
    }

    return false;
}

float kraken::CompoundObstacle::squaredDistance(const kraken::Vector2D &pos) const
{
    float min = std::numeric_limits<float>::max();
    for(auto const &o : obstacles_list_)
    {
        min = std::min(min, o.squaredDistance(pos));
        if(min == 0)
            break;
    }
    return min;
}

void kraken::CompoundObstacle::getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                                     std::vector<kraken::Vector2D> &vector_2d_list) const
{
    for(const auto &o : obstacles_list_)
        o.getExpandedConvexHull(expansion, longestAllowedLength, vector_2d_list);
}

bool kraken::CompoundObstacle::isColliding(const kraken::Vector2D &point_a, const kraken::Vector2D &point_b) const
{
    for(auto const &o : obstacles_list_)
    {
        if (o.isColliding(point_a, point_b))
            return true;
    }
    return false;
}

bool kraken::CompoundObstacle::isColliding(const kraken::RectangularObstacle &obs) const
{
    for(auto const &o : obstacles_list_)
    {
        if (o.isColliding(obs))
            return true;
    }
    return false;
}

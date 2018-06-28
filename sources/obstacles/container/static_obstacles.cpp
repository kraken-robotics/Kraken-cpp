#include "static_obstacles.h"

namespace kraken
{

    StaticObstacles::StaticObstacles(Vector2D bottom_left_corner, Vector2D top_right_corner) : bottom_left_corner_{
            bottom_left_corner}, top_right_corner_{top_right_corner}
    {

    }

    StaticObstacles::StaticObstacles(Vector2D bottom_left_corner, Vector2D top_right_corner,
                                     std::vector<std::shared_ptr<Obstacle>> o)
            : bottom_left_corner_{bottom_left_corner}, top_right_corner_{top_right_corner},
              obstacles_{std::move(o)}
    {

    }

    void StaticObstacles::add(std::shared_ptr<Obstacle> o)
    {
        obstacles_.emplace_back(std::move(o));
    }

    std::vector<std::shared_ptr<Obstacle>> StaticObstacles::getObstacles() const noexcept
    {
        return obstacles_;
    }

    Vector2D StaticObstacles::getBottomLeftCorner() const noexcept
    {
        return bottom_left_corner_;
    }

    Vector2D StaticObstacles::getTopRightCorner() const noexcept
    {
        return top_right_corner_;
    }

    bool StaticObstacles::isInsiderSearchDomain(const Vector2D &point) const noexcept
    {
        return point.getX() >= bottom_left_corner_.getX() && point.getX() <= top_right_corner_.getX() &&
               point.getY() >= bottom_left_corner_.getY() && point.getY() <= top_right_corner_.getY();
    }
}


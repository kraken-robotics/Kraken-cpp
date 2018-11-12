#include "obstacle.h"


namespace kraken
{
    Obstacle::Obstacle(const Vector2D &rotation_center) noexcept : rotation_center_(rotation_center)
    {}

    bool Obstacle::operator==(const Obstacle &rhs) const noexcept
    {
        return rotation_center_ == rhs.rotation_center_;
    }

    Vector2D Obstacle::getRotationCenter() const noexcept
    {
        return rotation_center_;
    }
}
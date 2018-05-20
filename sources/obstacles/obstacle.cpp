#include "obstacle.h"


namespace kraken
{

    Obstacle::Obstacle(const Vector2D &rotation_center) : rotation_center_(rotation_center)
    {
    }

    bool Obstacle::operator==(const Obstacle &rhs) const
    {
        return rotation_center_ == rhs.rotation_center_;
    }
}
#include "RectangularObstacle.h"
#include <cmath>
#include <vector>

namespace kraken
{

    RectangularObstacle::RectangularObstacle(const Vector2D &position, const int &size_x, const int &size_y) :
            RectangularObstacle(position, size_x, size_y, 0)
    {

    }

    RectangularObstacle::RectangularObstacle(const Vector2D &position, const int &size_x, const int &size_y,
                                             const float &angle) :
            RectangularObstacle(position, size_x / 2, size_x / 2, size_y / 2, size_y / 2, angle)
    {

    }

    RectangularObstacle::RectangularObstacle(const int &distance_to_front, const int &distance_to_back,
                                             const int &distance_to_left, const int &distance_to_right) :
            RectangularObstacle(distance_to_front, distance_to_back, distance_to_left, distance_to_right, 0)
    {

    }

    RectangularObstacle::RectangularObstacle(const int &distance_to_front, const int &distance_to_back,
                                             const int &distance_to_left, const int &distance_to_right,
                                             const float &tilt_angle) :
            RectangularObstacle(Vector2D(0, 0), distance_to_back, distance_to_front, distance_to_left,
                                distance_to_right, tilt_angle)
    {

    }

    RectangularObstacle::RectangularObstacle(const Vector2D &position, const int &size_left_x, const int &size_right_x,
                                             const int &size_up_y, const int &size_down_y, const float &angle) :
            RectangularObstacle(position, Vector2D(size_right_x, size_up_y), Vector2D(-size_left_x, -size_down_y),
                                angle)
    {

    }

    RectangularObstacle::RectangularObstacle(const Vector2D &position, const Vector2D &top_right_corner,
                                             const Vector2D &bottom_left_corner, const float &angle) :
            Obstacle(position), angle_(angle), cos_(std::cos(angle)), sin_(std::sin(angle)),
            left_bottom_corner_(bottom_left_corner),
            left_upper_corner_(bottom_left_corner.getX(), top_right_corner.getY()),
            right_bottom_corner_(top_right_corner.getX(), bottom_left_corner.getY()),
            right_upper_corner_(top_right_corner),
            left_bottom_corner_rotate_(toTableCoordinateSystem(left_bottom_corner_)),
            left_upper_corner_rotate_(toTableCoordinateSystem(left_upper_corner_)),
            right_bottom_corner_rotate_(toTableCoordinateSystem(right_bottom_corner_)),
            right_upper_corner_rotate_(toTableCoordinateSystem(right_upper_corner_)),
            geometric_center_((right_bottom_corner_rotate_ + left_upper_corner_rotate_) * 0.5f),
            half_diagonal_(top_right_corner.distance(bottom_left_corner) / 2.f)
    {

    }

    Vector2D RectangularObstacle::toObstacleCoordinateSystem(const Vector2D &point) const
    {
        Vector2D out;
        out.setX(getXToObstacleCoordinateSystem(point));
        out.setY(getYToObstacleCoordinateSystem(point));
        return out;
    }

    Vector2D RectangularObstacle::toTableCoordinateSystem(const Vector2D &point) const
    {
        Vector2D out;
        out.setX(cos_ * point.getX() - sin_ * point.getY() + rotation_center_.getX());
        out.setY(sin_ * point.getX() + cos_ * point.getY() + rotation_center_.getY());
        return out;
    }

    float RectangularObstacle::getXToObstacleCoordinateSystem(const Vector2D &point) const
    {
        return cos_ * (point.getX() - rotation_center_.getX()) + sin_ * (point.getY() - rotation_center_.getY());
    }

    float RectangularObstacle::getYToObstacleCoordinateSystem(const Vector2D &point) const
    {
        return -sin_ * (point.getX() - rotation_center_.getX()) + cos_ * (point.getY() - rotation_center_.getY());
    }

    void RectangularObstacle::getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                                    std::vector<Vector2D> &vector_2d_list) const
    {
        float coeff = expansion / half_diagonal_;
        Vector2D corners[] = {(right_bottom_corner_rotate_ - geometric_center_) * coeff + right_bottom_corner_rotate_,
                              (right_upper_corner_rotate_ - geometric_center_) * coeff + right_upper_corner_rotate_,
                              (left_upper_corner_rotate_ - geometric_center_) * coeff + left_upper_corner_rotate_,
                              (left_bottom_corner_rotate_ - geometric_center_) * coeff + left_bottom_corner_rotate_};

        vector_2d_list.clear();

        for (auto const &point : point)
            vector_2d_list.push_back(point);

        for (int i = 0; i < 4; i++)
        {
            Vector2D pointA = corners[i];
            Vector2D pointB = corners[(i + 1) % 4];
            float distance = pointA.distance(pointB);
            int nbPoints = static_cast<int>(std::ceil(distance / longestAllowedLength));
            float delta = distance / nbPoints;
            for (int j = 1; j < nbPoints; j++)
                vector_2d_list.push_back((pointB - pointA) * ((j * delta) / distance) + pointA);
        }
    }

    float RectangularObstacle::squaredDistance(const Vector2D &v) const
    {
        Vector2D in = toObstacleCoordinateSystem(v);

        if (in.getX() < left_bottom_corner_.getX() && in.getY() < left_bottom_corner_.getY())
            return in.squaredDistance(left_bottom_corner_);

        if (in.getX() < left_upper_corner_.getX() && in.getY() > left_upper_corner_.getY())
            return in.squaredDistance(left_upper_corner_);

        if (in.getX() > right_bottom_corner_.getX() && in.getY() < right_bottom_corner_.getY())
            return in.squaredDistance(right_bottom_corner_);

        if (in.getX() > right_upper_corner_.getX() && in.getY() > right_upper_corner_.getY())
            return in.squaredDistance(right_upper_corner_);

        if (in.getX() > right_upper_corner_.getX())
            return (in.getX() - right_upper_corner_.getX()) * (in.getX() - right_upper_corner_.getX());

        if (in.getX() < left_bottom_corner_.getX())
            return (in.getX() - left_bottom_corner_.getX()) * (in.getX() - left_bottom_corner_.getX());

        if (in.getY() > right_upper_corner_.getY())
            return (in.getY() - right_upper_corner_.getY()) * (in.getY() - right_upper_corner_.getY());

        if (in.getY() < left_bottom_corner_.getY())
            return (in.getY() - left_bottom_corner_.getY()) * (in.getY() - left_bottom_corner_.getY());

        // else, v is in the obstacle
        return 0;
    }

    float RectangularObstacle::getHalfDiagonal() const
    {
        return half_diagonal_;
    }
}
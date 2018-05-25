#include "rectangular_obstacle.h"
#include <cmath>
#include <vector>
#include <typeinfo>

namespace kraken
{
    RectangularObstacle::RectangularObstacle(const int &size_x, const int &size_y, const Vector2D &position,
                                             const float &angle) :
            RectangularObstacle(size_x / 2, size_x / 2, size_y / 2, size_y / 2, position, angle)
    {

    }

    RectangularObstacle::RectangularObstacle(const int &size_left_x, const int &size_right_x, const int &size_up_y,
                                             const int &size_down_y, const Vector2D &position, const float &angle) :
            RectangularObstacle(Vector2D(size_right_x, size_up_y), Vector2D(-size_left_x, -size_down_y), position,
                                angle)
    {

    }

    RectangularObstacle::RectangularObstacle(const Vector2D &top_right_corner, const Vector2D &bottom_left_corner,
                                             const Vector2D &position, const float &angle) :
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

    void RectangularObstacle::update(const Vector2D &position, const float &orientation)
    {
        update(position.getX(), position.getY(), orientation);
    }

    void RectangularObstacle::update(const float &x, const float &y, const float &orientation)
    {
        rotation_center_.setX(x);
        rotation_center_.setY(y);
        angle_ = orientation;
        cos_ = std::cos(angle_);
        sin_ = std::sin(angle_);
        left_bottom_corner_rotate_ = toTableCoordinateSystem(left_bottom_corner_);
        left_upper_corner_rotate_ = toTableCoordinateSystem(left_upper_corner_);
        right_bottom_corner_rotate_ = toTableCoordinateSystem(right_bottom_corner_);
        right_upper_corner_rotate_ = toTableCoordinateSystem(right_upper_corner_);

        geometric_center_ = (right_bottom_corner_rotate_ + left_upper_corner_rotate_) * 0.5f;
    }

    Vector2D RectangularObstacle::toObstacleCoordinateSystem(const Vector2D &point) const
    {
        return Vector2D(getXToObstacleCoordinateSystem(point), getYToObstacleCoordinateSystem(point));
    }

    Vector2D RectangularObstacle::toTableCoordinateSystem(const Vector2D &point) const
    {
        return Vector2D(cos_ * point.getX() - sin_ * point.getY() + rotation_center_.getX(),
                        sin_ * point.getX() + cos_ * point.getY() + rotation_center_.getY());
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

        for (auto const &point : corners)
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

    bool RectangularObstacle::isInObstacle(const Vector2D &pos) const
    {
        Vector2D in = toObstacleCoordinateSystem(pos);
        return in.getX() >= left_bottom_corner_.getX() && in.getX() <= right_upper_corner_.getX() &&
               in.getY() >= left_bottom_corner_.getY() && in.getY() <= right_upper_corner_.getY();
    }

    bool RectangularObstacle::isColliding(const Vector2D &point_a, const Vector2D &point_b) const
    {
        if (Vector2D::segmentIntersection(point_a, point_b, left_bottom_corner_rotate_, left_upper_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, left_upper_corner_rotate_, right_upper_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, right_upper_corner_rotate_, right_bottom_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, right_bottom_corner_rotate_, left_bottom_corner_rotate_))
            return true;

        return isInObstacle(point_a) || isInObstacle(point_b);
    }

    bool RectangularObstacle::isColliding(const RectangularObstacle &obs) const
    {
        if (rotation_center_.squaredDistance(obs.rotation_center_) >=
            (half_diagonal_ + obs.half_diagonal_) * (half_diagonal_ + obs.half_diagonal_))
            return false;

        return !test_separation(left_bottom_corner_.getX(), right_bottom_corner_.getX(),
                                getXToObstacleCoordinateSystem(obs.left_bottom_corner_rotate_),
                                getXToObstacleCoordinateSystem(obs.left_upper_corner_rotate_),
                                getXToObstacleCoordinateSystem(obs.right_bottom_corner_rotate_),
                                getXToObstacleCoordinateSystem(obs.right_upper_corner_rotate_))
               && !test_separation(left_bottom_corner_.getY(), left_upper_corner_.getY(),
                                   getYToObstacleCoordinateSystem(obs.left_bottom_corner_rotate_),
                                   getYToObstacleCoordinateSystem(obs.left_upper_corner_rotate_),
                                   getYToObstacleCoordinateSystem(obs.right_bottom_corner_rotate_),
                                   getYToObstacleCoordinateSystem(obs.right_upper_corner_rotate_))
               && !test_separation(obs.left_bottom_corner_.getX(), obs.right_bottom_corner_.getX(),
                                   obs.getXToObstacleCoordinateSystem(left_bottom_corner_rotate_),
                                   obs.getXToObstacleCoordinateSystem(left_upper_corner_rotate_),
                                   obs.getXToObstacleCoordinateSystem(right_bottom_corner_rotate_),
                                   obs.getXToObstacleCoordinateSystem(right_upper_corner_rotate_))
               && !test_separation(obs.left_bottom_corner_.getY(), obs.left_upper_corner_.getY(),
                                   obs.getYToObstacleCoordinateSystem(left_bottom_corner_rotate_),
                                   obs.getYToObstacleCoordinateSystem(left_upper_corner_rotate_),
                                   obs.getYToObstacleCoordinateSystem(right_bottom_corner_rotate_),
                                   obs.getYToObstacleCoordinateSystem(right_upper_corner_rotate_));
    }

    float RectangularObstacle::getHalfDiagonal() const
    {
        return half_diagonal_;
    }

    bool RectangularObstacle::operator==(const Obstacle &rhs) const
    {
        if (!Obstacle::operator==(rhs))
            return false;

        if (typeid(*this) != typeid(rhs))
            return false;

        RectangularObstacle ro_rhs = static_cast<const RectangularObstacle &>(rhs);
        return angle_ == ro_rhs.angle_ && left_bottom_corner_ == ro_rhs.left_bottom_corner_ &&
               right_upper_corner_ == ro_rhs.right_upper_corner_;
    }

    bool RectangularObstacle::test_separation(const float &a, const float &b, const float &a2, const float &b2,
                                              const float &c2, const float &d2) const
    {
        float min1 = std::min(a, b);
        float max1 = std::max(a, b);

        float min2 = std::min(std::min(a2, b2), std::min(c2, d2));
        float max2 = std::max(std::max(a2, b2), std::max(c2, d2));

        return min1 > max2 || min2 > max1;
    }

#if DEBUG

    std::ostream &operator<<(std::ostream &strm, const RectangularObstacle &o)
    {
        return strm << "RectangularObstacle(" << o.left_bottom_corner_rotate_ << " " << o.right_bottom_corner_rotate_
                    << " " << o.right_upper_corner_rotate_ << " " << o.left_upper_corner_rotate_ << ")" << std::endl;
    }

#endif

}
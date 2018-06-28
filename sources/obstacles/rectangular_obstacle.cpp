#include "rectangular_obstacle.h"
#include <vector>
#include <array>
#include <typeinfo>

namespace kraken
{
    RectangularObstacle::RectangularObstacle(const int &size_x, const int &size_y, const Vector2D &position,
                                             const float &angle) noexcept :
            RectangularObstacle(size_x / 2, size_x / 2, size_y / 2, size_y / 2, position, angle)
    {}

    RectangularObstacle::RectangularObstacle(const int &size_left_x, const int &size_right_x, const int &size_up_y,
                                             const int &size_down_y, const Vector2D &position,
                                             const float &angle) noexcept :
            RectangularObstacle(Vector2D(size_right_x, size_up_y), Vector2D(-size_left_x, -size_down_y), position,
                                angle)
    {}

    RectangularObstacle::RectangularObstacle(const Vector2D &top_right_corner, const Vector2D &bottom_left_corner,
                                             const Vector2D &position, const float &angle) noexcept :
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
    {}

    void RectangularObstacle::update(const Vector2D &position, const float &orientation) noexcept
    {
        update(position.getX(), position.getY(), orientation);
    }

    void RectangularObstacle::update(const float &x, const float &y, const float &orientation) noexcept
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

    void RectangularObstacle::getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                                    std::vector<Vector2D> &vector_2d_list) const
    {
        const auto coeff = expansion / half_diagonal_;
        std::array<const Vector2D, 4> corners = {
                (right_bottom_corner_rotate_ - geometric_center_) * coeff + right_bottom_corner_rotate_,
                (right_upper_corner_rotate_ - geometric_center_) * coeff + right_upper_corner_rotate_,
                (left_upper_corner_rotate_ - geometric_center_) * coeff + left_upper_corner_rotate_,
                (left_bottom_corner_rotate_ - geometric_center_) * coeff + left_bottom_corner_rotate_};

        for (const auto &point : corners)
            vector_2d_list.push_back(point);

        for (auto i = 0; i < 4; i++)
        {
            const auto pointA = corners[i];
            const auto pointB = corners[(i + 1) % 4];
            auto distance = pointA.distance(pointB);
            auto nbPoints = static_cast<int>(std::ceil(distance / longestAllowedLength));
            auto delta = distance / nbPoints;
            for (auto j = 1; j < nbPoints; j++)
                vector_2d_list.push_back((pointB - pointA) * ((j * delta) / distance) + pointA);
        }
    }

    float RectangularObstacle::squaredDistance(const Vector2D &v) const noexcept
    {
        auto position = toObstacleCoordinateSystem(v);

        if (position.getX() < left_bottom_corner_.getX() && position.getY() < left_bottom_corner_.getY())
            return position.squaredDistance(left_bottom_corner_);

        if (position.getX() < left_upper_corner_.getX() && position.getY() > left_upper_corner_.getY())
            return position.squaredDistance(left_upper_corner_);

        if (position.getX() > right_bottom_corner_.getX() && position.getY() < right_bottom_corner_.getY())
            return position.squaredDistance(right_bottom_corner_);

        if (position.getX() > right_upper_corner_.getX() && position.getY() > right_upper_corner_.getY())
            return position.squaredDistance(right_upper_corner_);

        if (position.getX() > right_upper_corner_.getX())
            return (position.getX() - right_upper_corner_.getX()) * (position.getX() - right_upper_corner_.getX());

        if (position.getX() < left_bottom_corner_.getX())
            return (position.getX() - left_bottom_corner_.getX()) * (position.getX() - left_bottom_corner_.getX());

        if (position.getY() > right_upper_corner_.getY())
            return (position.getY() - right_upper_corner_.getY()) * (position.getY() - right_upper_corner_.getY());

        if (position.getY() < left_bottom_corner_.getY())
            return (position.getY() - left_bottom_corner_.getY()) * (position.getY() - left_bottom_corner_.getY());

        // else, v is position the obstacle
        return 0;
    }

    bool RectangularObstacle::isInObstacle(const Vector2D &pos) const noexcept
    {
        const auto position = toObstacleCoordinateSystem(pos);
        return position.getX() >= left_bottom_corner_.getX() && position.getX() <= right_upper_corner_.getX() &&
               position.getY() >= left_bottom_corner_.getY() && position.getY() <= right_upper_corner_.getY();
    }

    bool RectangularObstacle::isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept
    {
        if (Vector2D::segmentIntersection(point_a, point_b, left_bottom_corner_rotate_, left_upper_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, left_upper_corner_rotate_, right_upper_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, right_upper_corner_rotate_, right_bottom_corner_rotate_)
            || Vector2D::segmentIntersection(point_a, point_b, right_bottom_corner_rotate_, left_bottom_corner_rotate_))
            return true;

        return isInObstacle(point_a) || isInObstacle(point_b);
    }

    bool RectangularObstacle::isColliding(const RectangularObstacle &obs) const noexcept
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

    float RectangularObstacle::getHalfDiagonal() const noexcept
    {
        return half_diagonal_;
    }

    bool RectangularObstacle::operator==(const Obstacle &rhs) const noexcept
    {
        if (!Obstacle::operator==(rhs))
            return false;

        if (typeid(*this) != typeid(rhs))
            return false;

        const auto ro_rhs = static_cast<const RectangularObstacle &>(rhs);
        return angle_ == ro_rhs.angle_ && left_bottom_corner_ == ro_rhs.left_bottom_corner_ &&
               right_upper_corner_ == ro_rhs.right_upper_corner_;
    }

    Vector2D RectangularObstacle::toObstacleCoordinateSystem(const Vector2D &point) const noexcept
    {
        return {getXToObstacleCoordinateSystem(point), getYToObstacleCoordinateSystem(point)};
    }

    Vector2D RectangularObstacle::toTableCoordinateSystem(const Vector2D &point) const noexcept
    {
        return {cos_ * point.getX() - sin_ * point.getY() + rotation_center_.getX(),
                sin_ * point.getX() + cos_ * point.getY() + rotation_center_.getY()};
    }

    float RectangularObstacle::getXToObstacleCoordinateSystem(const Vector2D &point) const noexcept
    {
        return cos_ * (point.getX() - rotation_center_.getX()) + sin_ * (point.getY() - rotation_center_.getY());
    }

    float RectangularObstacle::getYToObstacleCoordinateSystem(const Vector2D &point) const noexcept
    {
        return -sin_ * (point.getX() - rotation_center_.getX()) + cos_ * (point.getY() - rotation_center_.getY());
    }

    bool RectangularObstacle::test_separation(const float &a, const float &b, const float &a2, const float &b2,
                                              const float &c2, const float &d2) const noexcept
    {
        auto min1 = std::min(a, b);
        auto max1 = std::max(a, b);

        auto min2 = std::min(std::min(a2, b2), std::min(c2, d2));
        auto max2 = std::max(std::max(a2, b2), std::max(c2, d2));

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
#ifndef TESTS_RECTANGULAR_OBSTACLE_H
#define TESTS_RECTANGULAR_OBSTACLE_H

#include "obstacle.h"
#include "../struct/vector_2d.h"
#include <algorithm>

namespace kraken
{
    class RectangularObstacle : public Obstacle
    {
    public:
        RectangularObstacle(const int &size_x, const int &size_y, const Vector2D &position = {0, 0},
                            const float &angle = 0) noexcept;

        RectangularObstacle(const int &size_left_x, const int &size_right_x, const int &size_up_y,
                            const int &size_down_y, const Vector2D &position, const float &angle = 0) noexcept;

        RectangularObstacle(const Vector2D &top_right_corner, const Vector2D &bottom_left_corner,
                            const Vector2D &position, const float &angle = 0) noexcept;

        void update(const Vector2D &position, const float &orientation) noexcept;
        void update(const float &x, const float &y, const float &orientation) noexcept;

        void getExpandedConvexHull(const float &expansion, const float &longestAllowedLength,
                                   std::vector<Vector2D> &vector_2d_list) const override;
        float squaredDistance(const Vector2D &v) const noexcept override;
        bool isInObstacle(const Vector2D &pos) const noexcept override;
        bool isColliding(const Vector2D &point_a, const Vector2D &point_b) const noexcept override;
        bool isColliding(const RectangularObstacle &obs) const noexcept override;

        constexpr float getHalfDiagonal() const noexcept
        {
            return half_diagonal_;
        }

        bool operator==(const Obstacle &rhs) const noexcept override;

    protected:
        constexpr Vector2D toObstacleCoordinateSystem(const Vector2D &point) const noexcept
        {
            return {getXToObstacleCoordinateSystem(point), getYToObstacleCoordinateSystem(point)};
        }

        constexpr Vector2D toTableCoordinateSystem(const Vector2D &point) const noexcept
        {
            return {cos_ * point.getX() - sin_ * point.getY() + rotation_center_.getX(),
                    sin_ * point.getX() + cos_ * point.getY() + rotation_center_.getY()};
        }

        constexpr float getXToObstacleCoordinateSystem(const Vector2D &point) const
        {
            return cos_ * (point.getX() - rotation_center_.getX()) + sin_ * (point.getY() - rotation_center_.getY());
        }

        constexpr float getYToObstacleCoordinateSystem(const Vector2D &point) const
        {
            return -sin_ * (point.getX() - rotation_center_.getX()) + cos_ * (point.getY() - rotation_center_.getY());
        }

    private:
        bool test_separation(const float &a, const float &b, const float &a2, const float &b2, const float &c2,
                             const float &d2) const noexcept;

#if DEBUG
        friend std::ostream &operator<<(std::ostream &strm, const RectangularObstacle &o);
#endif

    protected:
        float angle_;
        float cos_;
        float sin_;

        // these corners are in the obstacle coordinate system
        const Vector2D left_bottom_corner_;
        const Vector2D left_upper_corner_;
        const Vector2D right_bottom_corner_;
        const Vector2D right_upper_corner_;

        // these corners are in the table coordinate system
        Vector2D left_bottom_corner_rotate_;
        Vector2D left_upper_corner_rotate_;
        Vector2D right_bottom_corner_rotate_;
        Vector2D right_upper_corner_rotate_;

        Vector2D geometric_center_;
        const float half_diagonal_;
    };
}

#endif //TESTS_RECTANGULAR_OBSTACLE_H

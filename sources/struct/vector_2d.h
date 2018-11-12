#ifndef KRAKEN_VECTOR_2D_H
#define KRAKEN_VECTOR_2D_H

#if DEBUG

#include <ostream>

#endif
#include <cmath>

namespace kraken
{

    class Vector2D
    {
    public:
        constexpr Vector2D() : x_(0.f), y_(0.f)
        {}

        constexpr Vector2D(const float &x, const float &y) : x_(x), y_(y)
        {}

        constexpr Vector2D operator+(const Vector2D &rhs) const noexcept
        {
            return {x_ + rhs.x_, y_ + rhs.y_};
        }

        constexpr Vector2D operator-(const Vector2D &rhs) const noexcept
        {
            return {x_ - rhs.x_, y_ - rhs.y_};
        }

        constexpr Vector2D operator*(const float &d) const noexcept
        {
            return {x_ * d, y_ * d};
        }

        constexpr bool operator==(const Vector2D &rhs) const noexcept
        {
            return x_ == rhs.x_ && y_ == rhs.y_;
        }

        constexpr bool operator!=(const Vector2D &rhs) const noexcept
        {
            return x_ != rhs.x_ || y_ != rhs.y_;
        }

        Vector2D &operator+=(const Vector2D &rhs) noexcept;
        Vector2D &operator-=(const Vector2D &rhs) noexcept;
        Vector2D &operator*=(const float &d) noexcept;

        constexpr float dot(const Vector2D &other) const noexcept
        {
            return x_ * other.x_ + y_ * other.y_;
        }

        constexpr float squaredDistance(const Vector2D &other) const noexcept
        {
            return (x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_);
        }

        float distance(const Vector2D &other) const;
        float distanceFast(const Vector2D &other) const noexcept;
        Vector2D &Ysym(bool do_symmetry) noexcept;
        Vector2D rotate(const float &angle, const Vector2D &rotation_center) const noexcept;
        void rotate(const float &angle, const Vector2D &rotation_center) noexcept;
        Vector2D &rotate(const float &angle) noexcept;
        Vector2D &rotate(const float &cos, const float &sin) noexcept;
        float getArgument() const;
        float getFastArgument() const;
        constexpr float squaredNorm() const noexcept
        {
            return x_ * x_ + y_ * y_;
        }

        float norm() const;

        /**
         * The distance is in μm !
         * @param other
         * @return
         */
        int distanceOctile(const Vector2D &other) const noexcept;

        constexpr float getX() const noexcept
        {
            return x_;
        }

        constexpr float getY() const noexcept
        {
            return y_;
        }

        void setX(float x);
        void setY(float y);
    public:
        /**
         * Returns true iff the segment (pointA1, pointA2) intersects the segment (pointB1, pointB2)
         * @param pointA1
         * @param pointA2
         * @param pointB1
         * @param pointB2
         * @return
         */
        static bool segmentIntersection(const Vector2D &point_A1, const Vector2D &point_A2, const Vector2D &point_B1,
                                        const Vector2D &point_B2) noexcept;

        static Vector2D fromPolar(float radius, float angle) noexcept;

    protected:
        float x_;
        float y_;

#if DEBUG
        friend std::ostream &operator<<(std::ostream &strm, const Vector2D &v);
#endif
    };
}

#endif //KRAKEN_VECTOR_2D_H

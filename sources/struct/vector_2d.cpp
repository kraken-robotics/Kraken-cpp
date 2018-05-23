#include <cmath>
#include <algorithm>
#include <cassert>
#include "vector_2d.h"

namespace kraken
{

    Vector2D::Vector2D() : x_(0.f), y_(0.f)
    {

    }

    Vector2D::Vector2D(const float &x, const float &y) : x_(x), y_(y)
    {

    }

    Vector2D Vector2D::operator+(const Vector2D &rhs) const
    {
        return Vector2D(x_ + rhs.x_, y_ + rhs.y_);
    }

    Vector2D &Vector2D::operator+=(const Vector2D &rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    Vector2D Vector2D::operator-(const Vector2D &rhs) const
    {
        return Vector2D(x_ - rhs.x_, y_ - rhs.y_);
    }

    Vector2D &Vector2D::operator-=(const Vector2D &rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    Vector2D Vector2D::operator*(const float &d) const
    {
        return Vector2D(x_ * d, y_ * d);
    }

    Vector2D &Vector2D::operator*=(const float &d)
    {
        x_ *= d;
        y_ *= d;
        return *this;
    }

    bool Vector2D::operator==(const Vector2D &rhs) const
    {
        return x_ == rhs.x_ && y_ == rhs.y_;
    }

    bool Vector2D::operator!=(const Vector2D &rhs) const
    {
        return x_ != rhs.x_ || y_ != rhs.y_;
    }

    float Vector2D::dot(const Vector2D &other) const
    {
        return x_ * other.x_ + y_ * other.y_;
    }

    float Vector2D::squaredDistance(const Vector2D &other) const
    {
        float tmp_x = x_ - other.x_, tmp_y = y_ - other.y_;
        return tmp_x * tmp_x + tmp_y * tmp_y;
    }

    float Vector2D::distance(const Vector2D &other) const
    {
        return std::sqrt(squaredDistance(other));
    }

    float Vector2D::distanceFast(const Vector2D &other) const
    {
        float dx = std::abs(x_ - other.x_);
        float dy = std::abs(y_ - other.y_);
        return std::max(dx, dy) + 0.414f * std::min(dx, dy);
    }

    Vector2D &Vector2D::Ysym(bool do_symmetry)
    {
        if (do_symmetry)
            y_ = -y_;
        return *this;
    }

    Vector2D Vector2D::rotate(float angle, const Vector2D &rotation_center) const
    {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        float x = cos * (x_ - rotation_center.x_) - sin * (y_ - rotation_center.y_) + rotation_center.x_;
        float y = sin * (x_ - rotation_center.x_) + cos * (y_ - rotation_center.y_) + rotation_center.y_;
        return Vector2D(x, y);
    }

    void Vector2D::rotate(float angle, const Vector2D &rotation_center)
    {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        float tmp_x = cos * (x_ - rotation_center.x_) - sin * (y_ - rotation_center.y_) + rotation_center.x_;
        y_ = sin * (x_ - rotation_center.x_) + cos * (y_ - rotation_center.y_) + rotation_center.y_;
        x_ = tmp_x;
    }

    Vector2D &Vector2D::rotate(float angle)
    {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        float old_x = x_;
        x_ = cos * x_ - sin * y_;
        y_ = sin * old_x + cos * y_;
        return *this;
    }

    Vector2D &Vector2D::rotate(float cos, float sin)
    {
        assert(std::abs(1 - cos * cos - sin * sin) < 0.01f);
        float old_x = x_;
        x_ = cos * x_ - sin * y_;
        y_ = sin * old_x + cos * y_;
        return *this;
    }

    float Vector2D::getArgument() const
    {
        return std::atan2(y_, x_);
    }

    float Vector2D::getFastArgument() const
    {
        // http://math.stackexchange.com/questions/1098487/atan2-faster-approximation
        double a = std::min(std::abs(x_), std::abs(y_)) / std::max(std::abs(x_), std::abs(y_));
        double s = a * a;
        double r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;
        if (std::abs(y_) > std::abs(x_))
            r = 1.57079637 - r;
        if (x_ < 0)
            r = 3.14159274 - r;
        if (y_ < 0)
            r = -r;
        return static_cast<float>(r);
    }

    float Vector2D::squaredNorm() const
    {
        return x_ * x_ + y_ * y_;
    }

    float Vector2D::norm() const
    {
        return std::sqrt(squaredNorm());
    }

    int Vector2D::distanceOctile(const Vector2D &other) const
    {
        float dx = std::abs(x_ - other.x_);
        float dy = std::abs(y_ - other.y_);
        return static_cast<int>(1000 * std::max(dx, dy) + 414 * std::min(dx, dy));
    }

    float Vector2D::getX() const
    {
        return x_;
    }

    float Vector2D::getY() const
    {
        return y_;
    }

    void Vector2D::setX(float x)
    {
        x_ = x;
    }

    void Vector2D::setY(float y)
    {
        y_ = y;
    }

    bool Vector2D::segmentIntersection(Vector2D &point_A1, Vector2D &point_A2, Vector2D &point_B1, Vector2D &point_B2)
    {
        // Source : https://stackoverflow.com/questions/3746274/line-intersection-with-aabb-rectangle

        Vector2D b = point_A2 - point_A1;
        Vector2D d = point_B2 - point_B1;
        float bDotDPerp = b.x_ * d.y_ - b.y_ * d.x_;

        // if b dot d == 0, it means the lines are parallel so have infinite intersection points
        if (bDotDPerp == 0)
            return false;

        Vector2D c = point_B1 - point_A1;
        float t = (c.x_ * d.y_ - c.y_ * d.x_) / bDotDPerp;
        if (t < 0 || t > 1)
            return false;

        float u = (c.x_ * b.y_ - c.y_ * b.x_) / bDotDPerp;

        if (u < 0 || u > 1)
            return false;

        return true;
    }

    Vector2D Vector2D::fromPolar(float radius, float angle)
    {
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        return Vector2D(x, y);
    }

#if DEBUG
    std::ostream &operator<<(std::ostream &strm, const Vector2D &v)
    {
        return strm << "Vector2D(" << v.x_ << "," << v.y_ << ")" << std::endl;
    }
#endif
}
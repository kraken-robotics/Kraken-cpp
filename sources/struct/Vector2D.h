#ifndef KRAKEN_VECTOR2D_H
#define KRAKEN_VECTOR2D_H

#include <ostream>

namespace kraken
{

    class Vector2D
    {
    public:
        Vector2D();
        Vector2D(float x, float y);
        Vector2D(const Vector2D &);

        Vector2D operator+(const Vector2D &rhs) const;
        Vector2D &operator+=(const Vector2D &rhs);
        Vector2D operator-(const Vector2D &rhs) const;
        Vector2D &operator-=(const Vector2D &rhs);
        Vector2D &operator*=(const float d);
        bool operator==(const Vector2D &rhs) const;
        float dot(const Vector2D &other) const;

        float squaredDistance(const Vector2D &other) const;
        float distance(const Vector2D &other) const;
        float distanceFast(const Vector2D &other) const;
        Vector2D &Ysym(bool do_symmetry);
        Vector2D rotate(float angle, Vector2D &rotation_center) const;
        void rotate(float angle, Vector2D &rotation_center);
        Vector2D &rotate(float angle);
        Vector2D &rotate(float cos, float sin);
        float getArgument() const;
        float getFastArgument() const;
        float squaredNorm() const;
        float norm() const;

        /**
         * The distance is in μm !
         * @param other
         * @return
         */
        int distanceOctile(const Vector2D &other) const;

        float getX() const;
        float getY() const;

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
        static bool segmentIntersection(Vector2D &pointA1, Vector2D &pointA2, Vector2D &pointB1, Vector2D &pointB2);
        static Vector2D fromPolar(float radius, float angle);
    private:
        float x_;
        float y_;

        friend std::ostream &operator<<(std::ostream &strm, const Vector2D &v);

    };
}

#endif //KRAKEN_VECTOR2D_H

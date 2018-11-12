#include "catch.hpp"
#include "../sources/obstacles/circular_obstacle.h"
#include "../sources/obstacles/rectangular_obstacle.h"

using namespace kraken;

TEST_CASE("Circular Obstacle", "[obstacles]")
{
    CircularObstacle o(Vector2D(0, 0), 30);
    REQUIRE(o.squaredDistance(Vector2D(10, 10)) == 0);
    REQUIRE(o.squaredDistance(Vector2D(22, 22)) > 0);
}

TEST_CASE("Rectangular Obstacle", "[obstacles]")
{
    {
        RectangularObstacle o(30, 10);
        REQUIRE(o.squaredDistance(Vector2D(13, -3)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(16, 0)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(-16, 0)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(0, 7)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(0, -7)) > 0);
    }

    {
        RectangularObstacle o(200, 200, Vector2D(0, 0), static_cast<float>(M_PI / 8));
        REQUIRE(o.squaredDistance(Vector2D(0, 0)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(100, 0)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(0, -100)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(90, 90)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(-90, -90)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(54, 130)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(-54, -130)) == 0);
        REQUIRE(o.squaredDistance(Vector2D(-54, 130)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(54, -130)) > 0);
        REQUIRE(o.squaredDistance(Vector2D(-100, 100)) > 0);
    }
}

TEST_CASE("Rectangular Colliding", "[obstacles]")
{
    RectangularObstacle o(200, 200, Vector2D(1000, 1000), static_cast<float>(M_PI / 8));
    REQUIRE(!o.isColliding(RectangularObstacle(10, 10, Vector2D(1200, 1000), 0)));
    REQUIRE(!o.isColliding(RectangularObstacle(10, 10, Vector2D(800, 1000), 0)));
    REQUIRE(!o.isColliding(RectangularObstacle(10, 10, Vector2D(0, 1200), 0)));
    REQUIRE(!o.isColliding(RectangularObstacle(10, 10, Vector2D(0, 800), 0)));
    REQUIRE(!o.isColliding(RectangularObstacle(20, 20, Vector2D(900, 1100), 0)));
    REQUIRE(!o.isColliding(RectangularObstacle(40, 40, Vector2D(900, 1100), 0)));
    REQUIRE(o.isColliding(RectangularObstacle(60, 60, Vector2D(900, 1100), 0)));
}

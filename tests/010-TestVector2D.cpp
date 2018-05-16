#include "catch.hpp"
#include <cmath>
#include "../sources/struct/Vector2D.h"

TEST_CASE("Vector2D", "[vector]")
{
    kraken::Vector2D a(10, 500);
    kraken::Vector2D b(20, -20);
    kraken::Vector2D c;
    kraken::Vector2D d(42, 890);

    REQUIRE (a == a);
    REQUIRE (a != b);

    REQUIRE ((a + b) == kraken::Vector2D(30, 480));
    REQUIRE ((a - b) == kraken::Vector2D(-10, 520));

    a += b;
    REQUIRE (a == kraken::Vector2D(30, 480));
    REQUIRE (a != d);

    d = a;
    REQUIRE (a == d);

    d += d;
    REQUIRE (a != d);

    d = kraken::Vector2D(a);
    REQUIRE (a == d);

    c -= b;
    REQUIRE (c == kraken::Vector2D(-20, 20));
    REQUIRE (c.squaredDistance(kraken::Vector2D()) == 800);
    REQUIRE (c.dot(a) == (-20 * 30 + 20 * 480));
    c.setX(4);
    c.setY(5);
    REQUIRE (c.distance(kraken::Vector2D(1, 1)) == 5);

    const kraken::Vector2D e(1, 0);
    REQUIRE (std::abs(kraken::Vector2D(0, 1).getX()
                      - e.rotate(M_PI / 2, kraken::Vector2D(0, 0)).getX()) < 0.1f);
}
#ifndef TESTS_STATICOBSTACLES_H
#define TESTS_STATICOBSTACLES_H

#include <vector>
#include <memory>
#include "../../struct/vector_2d.h"
#include "../obstacle.h"

namespace kraken
{

    class StaticObstacles
    {
    public:
        StaticObstacles(Vector2D bottom_left_corner, Vector2D top_right_corner);
        StaticObstacles(Vector2D bottom_left_corner, Vector2D top_right_corner, std::vector<std::shared_ptr<Obstacle>> o);
        void add(std::shared_ptr<Obstacle> o);
        std::vector<std::shared_ptr<Obstacle>> getObstacles() const noexcept;
        Vector2D getBottomLeftCorner() const noexcept;
        Vector2D getTopRightCorner() const noexcept;
        bool isInsiderSearchDomain(const Vector2D& point) const noexcept;

    private:
        Vector2D bottom_left_corner_;
        Vector2D top_right_corner_;
        std::vector<std::shared_ptr<Obstacle>> obstacles_;
    };

}


#endif //TESTS_STATICOBSTACLES_H

#ifndef TESTS_NAVMESH_TRIANGLE_H
#define TESTS_NAVMESH_TRIANGLE_H

#include "navmesh_edge.h"
#include <vector>
#include <memory>


namespace kraken
{
    class NavmeshTriangle : public std::enable_shared_from_this<NavmeshTriangle>
    {
    public:
        NavmeshTriangle(const NavmeshEdge &a, const NavmeshEdge &b, const NavmeshEdge &c);
        bool checkCounterclockwise() const;
        bool checkUniquePoints() const;
        bool checkDuality() const;
        void setEdges(const NavmeshEdge &e1, const NavmeshEdge &e2, const NavmeshEdge &e3);
        void correctCounterclockwiseness();
        bool isInside(const Vector2D &pos);
        bool operator==(const NavmeshTriangle &tr);

    private:
        void updateArea();
        bool checkTriangle(const NavmeshEdge &e1, const NavmeshEdge &e2, const NavmeshEdge &e3) const;

        std::vector<std::shared_ptr<NavmeshNode>> points_;
        std::vector<std::weak_ptr<NavmeshEdge>> edges_;
        int* edgesNb_;
        int area_;
    };

}


#endif //TESTS_NAVMESH_TRIANGLE_H

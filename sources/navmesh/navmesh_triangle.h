#ifndef TESTS_NAVMESH_TRIANGLE_H
#define TESTS_NAVMESH_TRIANGLE_H

#include "navmesh_edge.h"

namespace kraken
{
    class NavmeshTriangle
    {
    public:
        NavmeshTriangle(const NavmeshEdge &a, const NavmeshEdge &b, const NavmeshEdge &c);
    };

}


#endif //TESTS_NAVMESH_TRIANGLE_H

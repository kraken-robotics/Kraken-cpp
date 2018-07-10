#ifndef TESTS_NAVMESH_EDGE_H
#define TESTS_NAVMESH_EDGE_H

#include "navmesh_node.h"
#include "navmesh_triangle.h"
#include "../obstacles/obstacle.h"
#include <vector>
#include <memory>

namespace kraken
{
    class NavmeshEdge : public std::enable_shared_from_this<NavmeshEdge>
    {
    public:
        NavmeshEdge(const NavmeshNode &p1, const NavmeshNode &p2);

        void updateOrientation();
        void updateLength();
        void addTriangle(const NavmeshTriangle &tr);
        void removeTriangle(const NavmeshTriangle &tr);
        void replaceTriangle(const NavmeshTriangle &old_tr, const NavmeshTriangle &new_tr);
        void updateState(const std::vector<Obstacle *> &current_list);
        bool flipIfNecessary();
        bool forceFlip();

        int getNbTriangles() const;
        bool checkTriangle(int expected) const;
        float distanceToPoint(const NavmeshNode &next_node) const;
        float distanceToPointTie(const NavmeshNode &next_node) const;
        int getDistance() const;
        int getUnblockedDistance() const;
        bool isBlocked() const;
        bool isAdjacent(const NavmeshEdge &p) const;
        float getOrientation(const NavmeshNode &origin) const;
        bool containsNode(const NavmeshNode &p) const;
        std::shared_ptr<NavmeshNode> getPoint(int index) const;

        bool operator==(const NavmeshEdge &rhs) const;

    private:
        bool flip(bool force);
        bool checkNbTriangles() const;

    private:
        int length_;
        int nb_;
        float orientation_;
        std::vector<std::shared_ptr<NavmeshNode>> points_;
        std::vector<std::shared_ptr<NavmeshTriangle>> triangles_;
        std::vector<std::shared_ptr<Obstacle>> obstructing_obstacles_;
    };

}

#endif //TESTS_NAVMESH_EDGE_H

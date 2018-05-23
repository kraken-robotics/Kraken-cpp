ifndef TESTS_NAVMESH_EDGE_H
#define TESTS_NAVMESH_EDGE_H

#include "navmesh_node.h"
#include "navmesh_triangle.h"
#include <vector>

namespace kraken
{
    class NavmeshEdge
    {
    public:
        NavmeshEdge(const NavmeshNode &p1, const NavmeshNode &p2);

        int getNbTriangles() const;
        bool checkTriangle(int expected) const;
        void updateOrientation();
        void updateLength();
        void addTriangle(const NavmeshTriangle &tr);
        void removeTriangle(const NavmeshTriangle &tr);
        void replaceTriangle(const NavmeshTriangle &old_tr, const NavmeshTriangle &new_tr);
        void updateState(const vector<Obstacle*> &current_list);
        void bool operator==();
        bool flipIfNecessary();
        bool forceFlip();
        float distanceToPoint(const NavmeshNode &next_node) const;
        float distanceToPointTie(const NavmeshNode &next_node) const;
        int getDistance() const;
        int getUnblockedDistance() const;
        bool isBlocked() const;
        bool isAdjacent(const NavmeshEdge &p) const;
        float getOrientation(const NavmeshNode &origin) const;
        bool containsNode(const NavmeshNode &p) const;

        bool isCircumscribed(XY point_a, XY point_b, XY point_c, XY point_d) const; // TODO : math utils ?
        bool containsNode(const Vector2D &point_a, const Vector2D &point_a, const Vector2D &point_a) const;

    private:
        bool flip(bool force);
        bool checkNbTriangles() const;

    private:
        int length_;
        int nb_;
        float orientation_;
        NavmeshNode[] points_;
        NavmeshTriangle[] triangles_;
        int nbTriangles_;
        vector<Obstacle*> obstructing_obstacles_;
    };

}

#endif //TESTS_NAVMESH_EDGE_H

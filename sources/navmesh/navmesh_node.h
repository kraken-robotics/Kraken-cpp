#ifndef TESTS_NAVMESH_NODE_H
#define TESTS_NAVMESH_NODE_H

#include <vector>

namespace kraken
{
    class NavmeshEdge;

    class NavmeshNode
    {
    public:
        NavmeshNode(const Vector2D &pos);

        void addEdge(const NavmeshEdge &e);
        void removeEdge(const NavmeshEdge &e);
        void updateNeighbours();

        int getNbNeighbours() const;
        NavmeshEdge getNeighbourEdge(int index) const;
        NavmeshNode getNeighbourNode(int index) const;
        bool isNeighbourOf(const NavmeshNode &other) const;

        bool operator==(const NavmeshNode &rhs) const;

    private:
        static int nb_total_;
        int nb_;
        Vector2D pos_;
        std::vector<NavmeshEdge*> edges_;
        std::vector<int> edges_nb_;
        std::vector<NavmeshNode*> neighbours_;
    };

}

#endif //TESTS_NAVMESH_NODE_H

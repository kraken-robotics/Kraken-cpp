#ifndef TESTS_NAVMESH_NODE_H
#define TESTS_NAVMESH_NODE_H

#include "../struct/vector_2d.h"
#include <vector>
#include <set>
#include <memory>

namespace kraken
{
    class NavmeshEdge;

    class NavmeshNode
    {
    public:
        NavmeshNode(const Vector2D &pos);
        NavmeshNode(NavmeshNode&& navmeshNode) = default;

        void addEdge(const NavmeshEdge &e) noexcept;
        void removeEdge(const NavmeshEdge &e) noexcept;
        void updateNeighbours() noexcept (false);

        std::set<NavmeshEdge>::size_type getNbNeighbours() const noexcept (false);
        NavmeshEdge getNeighbourEdge(int index) const;
        std::shared_ptr<NavmeshNode> getNeighbourNode(int index) const;
        bool isNeighbourOf(const NavmeshNode &other) const;

        bool operator==(const NavmeshNode &rhs) const noexcept;

    private:
        static int nb_total_;
        int nb_;
        Vector2D position_;
        std::set<NavmeshEdge> edges_;
        std::vector<int> edges_nb_;
        std::set<std::shared_ptr<NavmeshNode>> neighbours_;
    };

}

#endif //TESTS_NAVMESH_NODE_H

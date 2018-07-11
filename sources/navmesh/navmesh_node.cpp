#include <algorithm>
#include "navmesh_node.h"
#include "navmesh_edge.h"

namespace kraken
{

    int NavmeshNode::nb_total_ = 0;

    NavmeshNode::NavmeshNode(const Vector2D &pos) : nb_(nb_total_++), position_(pos)
    {

    }

    void NavmeshNode::addEdge(const NavmeshEdge &e) noexcept
    {
        edges_.emplace(e);
    }

    void NavmeshNode::removeEdge(const NavmeshEdge &e) noexcept
    {
        edges_.erase(e);
    }

    void NavmeshNode::updateNeighbours() noexcept(false)
    {
        neighbours_.clear();
        for(const auto & e : edges_)
        {
            if(e.getPoint(0).get() == this)
            {
                neighbours_.emplace(e.getPoint(1));
            }
            else if (e.getPoint(1).get() == this)
            {
                neighbours_.emplace(e.getPoint(0));
            }
            else
                throw std::runtime_error("Node not found in the NavmeshEdge");
        }
    }

    std::set<NavmeshEdge>::size_type NavmeshNode::getNbNeighbours() const noexcept (false)
    {
        if (neighbours_.size() != edges_.size())
            throw std::runtime_error("Size is different");
        return edges_.size();
    }

    NavmeshEdge NavmeshNode::getNeighbourEdge(int index) const
    {
        auto it = edges_.begin();
        std::advance(it, index);
        return *it;
    }

    std::shared_ptr<NavmeshNode> NavmeshNode::getNeighbourNode(int index) const
    {
        auto it = neighbours_.begin();
        std::advance(it, index);
        return *it;
    }

    bool NavmeshNode::isNeighbourOf(const NavmeshNode &other) const
    {
        auto it = std::find_if(neighbours_.cbegin(), neighbours_.cend(),
                     [&other](std::shared_ptr<NavmeshNode> const& i){ return *(i.get()) == other; });

        return it != neighbours_.cend();
    }

    bool NavmeshNode::operator==(const NavmeshNode &rhs) const noexcept
    {
        return rhs.nb_ == nb_;
    }
}
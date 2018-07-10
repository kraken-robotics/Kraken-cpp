#include "navmesh_node.h"
#include "navmesh_edge.h"

namespace kraken
{

    int NavmeshNode::nb_total_ = 0;

    NavmeshNode::NavmeshNode(const Vector2D &pos) : position_(pos), nb_(nb_total_++)
    {

    }

    void NavmeshNode::addEdge(const NavmeshEdge &e)
    {
        edges_.emplace(e);
    }

    void NavmeshNode::removeEdge(const NavmeshEdge &e)
    {
        edges_.erase(e);
    }

    void NavmeshNode::updateNeighbours()
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

    std::set<NavmeshEdge>::size_type NavmeshNode::getNbNeighbours() const
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

    NavmeshNode NavmeshNode::getNeighbourNode(int index) const
    {
        auto it = neighbours_.begin();
        std::advance(it, index);
        return *it;
    }

    bool NavmeshNode::isNeighbourOf(const NavmeshNode &other) const
    {
        return neighbours_.find(other) != neighbours_.end();
    }

    bool NavmeshNode::operator==(const NavmeshNode &rhs) const
    {
        return rhs.nb_ == nb_;
    }
}
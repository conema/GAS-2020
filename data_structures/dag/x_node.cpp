#include "x_node.h"

dag::XNode::XNode(const size_t &pointId):
    Node(dag::NodeType::XNODE),
    pointId(pointId)
{

}

size_t dag::XNode::getPointId() const
{
    return pointId;
}

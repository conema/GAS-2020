#include "x_node.h"

dag::XNode::XNode(const size_t &pointId):
    Node(dag::NodeType::XNODE),
    pointId(pointId)
{

}

/**
 * @brief Get the point id of the #XNode
 * @return the point id
 */
size_t dag::XNode::getPointId() const
{
    return pointId;
}

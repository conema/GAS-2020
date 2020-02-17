#include "x_node.h"

dag::XNode::XNode(const size_t &pointId):
    Node(dag::NodeType::XNODE),
    pointId(pointId)
{

}

#include "y_node.h"

dag::YNode::YNode(const size_t &segmentId):
    Node(dag::NodeType::YNODE),
    segmentId(segmentId)
{

}

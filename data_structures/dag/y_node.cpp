#include "y_node.h"

dag::YNode::YNode(const size_t &segmentId):
    Node(dag::NodeType::YNODE),
    segmentId(segmentId)
{

}

size_t dag::YNode::getSegmentId() const
{
    return segmentId;
}

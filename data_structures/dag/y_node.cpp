#include "y_node.h"

dag::YNode::YNode(const size_t &segmentId):
    Node(dag::NodeType::YNODE),
    segmentId(segmentId)
{

}

/**
 * @brief get the segment id of the #YNode
 * @return the segment id
 */
size_t dag::YNode::getSegmentId() const
{
    return segmentId;
}

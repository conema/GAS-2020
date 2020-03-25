#include "dag.h"

dag::Dag::Dag():
    root(nullptr)
{

}

dag::Dag::Dag(dag::Leaf *node):
    root(node)
{
    node->getTrapezoid()->setLeaf(node);
}

dag::Dag::Dag(dag::Node *node):
    root(node)
{
}

dag::Dag::~Dag()
{
    deleteGraph(root);
}

void dag::Dag::initializeDag(dag::Leaf *node)
{
    node->getTrapezoid()->setLeaf(node);
    root = node;
}

void dag::Dag::swap(dag::Dag &other)
{
    using std::swap;
    swap(root, other.root);
}

dag::Dag::Dag(const dag::Dag &dag)
{
    root = new Node(*(dag.root));
}

/*dag::Dag &dag::Dag::operator =(dag::Dag other)
{
    swap(other);
    return *this;
}*/

dag::Dag &dag::Dag::operator =(dag::Dag &&other)
{
    std::swap(root, other.root);
    return *this;
}

dag::Dag::Dag(dag::Dag &&in)
{
    this->root = in.root;
    in.root = nullptr;
}

void dag::Dag::setRoot(Node *value)
{
    root = value;
}

dag::Node *dag::Dag::getRoot() const
{
    return root;
}

dag::Leaf *dag::Dag::getLeaf(dag::Node *node) const
{
    return reinterpret_cast<dag::Leaf*>(node);
}

dag::XNode *dag::Dag::getXNode(dag::Node *node) const
{
    return reinterpret_cast<dag::XNode*>(node);
}

dag::YNode *dag::Dag::getYNode(dag::Node *node) const
{
    return reinterpret_cast<dag::YNode*>(node);
}

/**
 * @brief Delete all the node of the DAG
 * @param[in] *node: pointer to a node
 */
void dag::Dag::deleteGraph(Node *node)
{
    if (node == nullptr){
        return;
    }

    deleteGraph(node->getLeftChild());
    deleteGraph(node->getRightChild());

    Node copyNode = *node;

    delete node;

    updateChildren(&copyNode, nullptr);
    node = nullptr;
}

/**
 * @brief Find a point into a trapezoid
 * @param[in] *node: pointer to a node
 * @param[in] *trapezoidalMapDataset: the trapezoidal map dataset to find points and segments
 * @param[in] &segment: the point to search
 */
dag::Leaf *dag::Dag::findPoint(Node *node, const TrapezoidalMapDataset &trapezoidalMapDataset, const cg3::Segment2d &segment) const
{
    if (node == nullptr){
        return nullptr;
    }

    switch (node->getNodeType())
    {
        case dag::NodeType::LEAF:
            return getLeaf(node);
        case dag::NodeType::XNODE:
        {
            const cg3::Point2d &nodePoint = trapezoidalMapDataset.getPoint(getXNode(node)->getPointId());

            // Test if the point is on the left or on the right of nodePoint
            if (nodePoint.x() > segment.p1().x()){
                return findPoint(node->getLeftChild(), trapezoidalMapDataset, segment);
            } else {
                return findPoint(node->getRightChild(), trapezoidalMapDataset, segment);
            }
            break;
        }
        case dag::NodeType::YNODE:
        {
            const cg3::Segment2d &nodeSegment = trapezoidalMapDataset.getSegment(getYNode(node)->getSegmentId());

            // Test if the point is equal to one endpoint
            if(nodeSegment.p2() == segment.p1() || nodeSegment.p2() == segment.p2() || nodeSegment.p1() == segment.p1() || nodeSegment.p1() == segment.p2()){
                // Check to see if the new segment is under or over the y-node (we can't use findPointSide because one point is equal)
                if (tmap::findPointSide(nodeSegment, tmap::differentPoint(segment, nodeSegment))){
                    return findPoint(node->getLeftChild(), trapezoidalMapDataset, segment);
                } else {
                    return findPoint(node->getRightChild(), trapezoidalMapDataset, segment);
                }
            }

            // Test if the point is on the left or on the right of nodePoint
            if(tmap::findPointSide(nodeSegment, segment.p1())){
                return findPoint(node->getLeftChild(), trapezoidalMapDataset, segment);
            } else {
                return findPoint(node->getRightChild(), trapezoidalMapDataset, segment);
            }

            assert(segment.p1().x() < segment.p2().x());
        }
    }

    // Never reachable
    return nullptr;
}

/**
 * @brief Update the children
 * @param[in] fathers : a list of fathers
 * @param[in] oldChildren : the children to substitute
 * @param[in] newChildren : the substitute
 */
void dag::Dag::updateChildren(dag::Node* oldChildren, dag::Node* newChildren){
    for (const auto& father: oldChildren->getFathers()){
        if (father->getLeftChild() != nullptr && father->getLeftChild() == oldChildren){
            // The oldChildren is a left child
            father->setLeftChild(newChildren);
        } else if(father->getRightChild() != nullptr) {
            // The oldChildren is a right child
            father->setRightChild(newChildren);
        }
    }
}

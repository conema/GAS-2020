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


// Rule of 5
void dag::Dag::swap(dag::Dag &other)
{
    using std::swap;
    swap(root, other.root);
}

dag::Dag::Dag(const dag::Dag &dag)
{
    root = createDag(dag.getRoot());
}

dag::Dag &dag::Dag::operator =(dag::Dag other)
{
    swap(other);
    return *this;
}

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


/**
 * @brief Duplicate a DAG
 * @param[in] *oldNode: pointer to the oldNode to be copied
 * @return the pointer of the new node
 */
dag::Node* dag::Dag::createDag(dag::Node *oldNode){
    dag::Node *node = nullptr;
    dag::XNode *xNode = nullptr;
    dag::YNode *yNode = nullptr;
    dag::Leaf *leaf = nullptr;

    // Find node type and create a node with the correct encapsulation
    switch(oldNode->getNodeType()){
        case NodeType::XNODE:
            xNode = new dag::XNode(getXNode(oldNode)->getPointId());
            node = xNode;
            break;
        case NodeType::YNODE:
            yNode = new dag::YNode(getYNode(oldNode)->getSegmentId());
            node = yNode;
            break;
        case NodeType::LEAF:
            leaf = new dag::Leaf(getLeaf(oldNode)->getTrapezoid());
            node = leaf;

            // Return the leaf because it hasn't children
            return node;
    }

    dag::Node *newLeftChild = createDag(oldNode->getLeftChild());
    dag::Node *newRightChild = createDag(oldNode->getRightChild());

    // Add the children to the new copied node
    node->setLeftChild(newLeftChild);
    node->setRightChild(newRightChild);

    // Set the new copied node as father
    newLeftChild->getFathers().insert(node);
    newRightChild->getFathers().insert(node);

    return node;
}

/**
 * @brief set the root of the DAG
 * @param value
 */
void dag::Dag::setRoot(Node *value)
{
    root = value;
}

/**
 * @brief get the root of the DAG
 * @return a node
 */
dag::Node *dag::Dag::getRoot() const
{
    return root;
}

/**
 * @brief cast the node to #Leaf
 * @param node
 * @return a #Leaf
 */
dag::Leaf *dag::Dag::getLeaf(dag::Node *node) const
{
    return reinterpret_cast<dag::Leaf*>(node);
}

/**
 * @brief cast the node to #XNode
 * @param node
 * @return an #XNode
 */
dag::XNode *dag::Dag::getXNode(dag::Node *node) const
{
    return reinterpret_cast<dag::XNode*>(node);
}

/**
 * @brief cast the node to #YNode
 * @param node
 * @return an #YNode
 */
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

    updateChildren(node, nullptr);

    deleteNode(node);
}

/**
 * @brief Delete one node from the DAG
 * @param[in] *node: pointer to a node
 */
void dag::Dag::deleteNode(Node *node) {
    if (node->getNodeType() == NodeType::LEAF){
        Leaf *leaf = Dag::getLeaf(node);
        if (leaf->getTrapezoid() != nullptr){
            leaf->getTrapezoid()->setLeaf(nullptr);
        }

        delete leaf;
    } else if (node->getNodeType() == NodeType::XNODE){
        delete Dag::getXNode(node);
    } else if(node->getNodeType() == NodeType::YNODE){
        delete Dag::getYNode(node);
    }
}

/**
 * @brief Find a point into a trapezoid
 * @param[in] *node: pointer to a node
 * @param[in] *trapezoidalMap: the trapezoidal map to find points and segments
 * @param[in] &segment: the point to search
 * @return the found leaf
 */
dag::Leaf *dag::Dag::findPoint(Node *node, const tmap::TrapezoidalMap &trapezoidalMap, const cg3::Segment2d &segment) const
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
            const cg3::Point2d &nodePoint = trapezoidalMap.getPoint(getXNode(node)->getPointId());

            // Test if the point is on the left or on the right of nodePoint
            if (nodePoint.x() > segment.p1().x()){
                return findPoint(node->getLeftChild(), trapezoidalMap, segment);
            } else {
                return findPoint(node->getRightChild(), trapezoidalMap, segment);
            }
            break;
        }
        case dag::NodeType::YNODE:
        {
            const cg3::Segment2d &nodeSegment = trapezoidalMap.getSegment(getYNode(node)->getSegmentId());

            // Test if the point is equal to one endpoint
            if(nodeSegment.p2() == segment.p1() || nodeSegment.p2() == segment.p2() || nodeSegment.p1() == segment.p1() || nodeSegment.p1() == segment.p2()){
                // Check to see if the new segment is under or over the y-node
                if (tmaputils::findPointSide(nodeSegment, tmaputils::differentPoint(segment, nodeSegment))){
                    return findPoint(node->getLeftChild(), trapezoidalMap, segment);
                } else {
                    return findPoint(node->getRightChild(), trapezoidalMap, segment);
                }
            }

            // Test if the point is on the left or on the right of nodePoint
            if(tmaputils::findPointSide(nodeSegment, segment.p1())){
                return findPoint(node->getLeftChild(), trapezoidalMap, segment);
            } else {
                return findPoint(node->getRightChild(), trapezoidalMap, segment);
            }

            assert(segment.p1().x() < segment.p2().x());
        }
    }

    // Never reachable
    assert(false);

    return nullptr;
}

/**
 * @brief Update the children of a #Node
 * @param[in] oldChild: the child to substitute
 * @param[in] newChild: the substitute
 */
void dag::Dag::updateChildren(dag::Node* oldChild, dag::Node* newChild){
    for (const auto& father: oldChild->getFathers()){
        if (father->getLeftChild() != nullptr && father->getLeftChild() == oldChild){
            // The oldChildren is a left child
            father->setLeftChild(newChild);
        } else if(father->getRightChild() != nullptr) {
            // The oldChildren is a right child
            father->setRightChild(newChild);
        }
    }
}

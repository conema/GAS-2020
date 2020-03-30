#include "node.h"

dag::Node::Node(dag::NodeType nodeType):
    rightChild(nullptr),
    leftChild(nullptr),
    nodeType(nodeType)
{

}

/**
 * @brief get the right child of the node
 * @return a node
 */
dag::Node *dag::Node::getRightChild() const
{
    return rightChild;
}

/**
 * @brief set the right child of the node
 * @param value
 */
void dag::Node::setRightChild(Node *value)
{
    rightChild = value;
}

/**
 * @brief get the left child of the node
 * @return a node
 */
dag::Node *dag::Node::getLeftChild() const
{
    return leftChild;
}

/**
 * @brief set the left child of the node
 * @param value
 */
void dag::Node::setLeftChild(Node *value)
{
    leftChild = value;
}

/**
 * @brief get the type of node
 * @return the #NodeType
 */
dag::NodeType dag::Node::getNodeType() const
{
    return nodeType;
}

/**
 * @brief set the node tipe
 * @param value
 */
void dag::Node::setNodeType(const NodeType &value)
{
    nodeType = value;
}

/**
 * @brief dag::Node::getFathers
 * @return the fathers of the node
 */
std::unordered_set<dag::Node *>& dag::Node::getFathers()
{
    return fathers;
}

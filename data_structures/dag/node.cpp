#include "node.h"

dag::Node::Node(dag::NodeType nodeType):
    rightChild(nullptr),
    leftChild(nullptr),
    nodeType(nodeType)
{

}

dag::Node *dag::Node::getRightChild() const
{
    return rightChild;
}

void dag::Node::setRightChild(Node *value)
{
    rightChild = value;
}

dag::Node *dag::Node::getLeftChild() const
{
    return leftChild;
}

void dag::Node::setLeftChild(Node *value)
{
    leftChild = value;
}

dag::NodeType dag::Node::getNodeType() const
{
    return nodeType;
}

void dag::Node::setNodeType(const NodeType &value)
{
    nodeType = value;
}

std::unordered_set<dag::Node *>& dag::Node::getFathers()
{
    return fathers;
}

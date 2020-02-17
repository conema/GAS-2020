#include "node.h"

dag::Node::Node()
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

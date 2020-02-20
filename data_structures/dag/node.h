#ifndef NODE_H
#define NODE_H

#include <cg3/geometry/point2.h>

namespace dag
{
    enum NodeType {
        LEAF,
        YNODE,
        XNODE
    };

    class Node
    {
    public:
        Node(dag::NodeType nodeType);

        Node *getRightChild() const;
        void setRightChild(Node *value);

        Node *getLeftChild() const;
        void setLeftChild(Node *value);

        NodeType getNodeType() const;
        void setNodeType(const NodeType &value);

        std::unordered_set<dag::Node *>& getFathers();
    private:
        Node *rightChild;
        Node *leftChild;
        std::unordered_set<dag::Node*> fathers;
        NodeType nodeType;
    };
}

#endif // NODE_H

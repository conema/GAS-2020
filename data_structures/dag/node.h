#ifndef NODE_H
#define NODE_H

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

    private:
        Node *rightChild;
        Node *leftChild;
        NodeType nodeType;
    };
}

#endif // NODE_H

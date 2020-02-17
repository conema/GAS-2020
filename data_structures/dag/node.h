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
        Node();
        Node *getRightChild() const;
        void setRightChild(Node *value);

        Node *getLeftChild() const;
        void setLeftChild(Node *value);

    private:
        Node *rightChild;
        Node *leftChild;
        NodeType nodeType;
    };
}

#endif // NODE_H

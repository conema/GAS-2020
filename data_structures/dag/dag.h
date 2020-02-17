#ifndef DAG_H
#define DAG_H


#include <cg3/geometry/segment2.h>
#include <data_structures/tmap/trapezoid.h>
#include "node.h"
#include "leaf.h"

namespace dag
{

    class Dag
    {
    public:
        Dag(Leaf *node);

        //Node* findNode(const Node &node);
        void setRoot(Node *value);

        Node *getRoot() const;

    private:
        Node *root;

        //void removeLeaf(Leaf &leaf);
    };
}

#endif // DAG_H

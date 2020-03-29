#ifndef DAG_H
#define DAG_H


#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include <data_structures/tmap/trapezoid.h>
#include <data_structures/tmap/trapezoidalmap.h>
#include <algorithms/trapezoidalmap_helper.h>
#include "node.h"
#include "leaf.h"
#include "x_node.h"
#include "y_node.h"

namespace dag
{

    class Dag
    {
    public:
        Dag();
        Dag(Leaf *node);
        Dag(Node *node);
        Dag(const Dag& dag);
        //Dag& operator = (Dag other);
        Dag& operator = (Dag&& other);
        Dag(Dag&& in);
        ~Dag();

        void initializeDag(Leaf *node);

        void swap(Dag& other);

        Leaf* findPoint(Node *node,
                       const tmap::TrapezoidalMap &trapezoidalMap,
                       const cg3::Segment2d &segment) const;

        void setRoot(Node *value);
        Node *getRoot() const;

        Leaf* getLeaf(Node *node) const;
        XNode* getXNode(Node *node) const;
        YNode* getYNode(Node *node) const;

        void deleteGraph(Node *node);
        void deleteNode(Node *node);

        void updateChildren(Node* oldChildren, Node* newChildren);
    private:
        Node *root;
    };
}

#endif // DAG_H

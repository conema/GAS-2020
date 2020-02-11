#ifndef DAG_H
#define DAG_H

#include "x_node.h"
#include <cg3/geometry/segment2.h>
#include "leaf.h"

namespace dag
{

    class dag
    {
    public:
        dag();

        void addSegment(const cg3::Segment2d &segment);
    private:
        x_node root;

        void removeLeaf(Leaf &leaf);
    };
}

#endif // DAG_H

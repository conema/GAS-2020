#ifndef DAG_H
#define DAG_H


#include <cg3/geometry/segment2.h>
#include "node.h"

namespace dag
{

    class dag
    {
    public:
        dag();

        void addSegment(const cg3::Segment2d &segment);
    private:
        Node root;

        //void removeLeaf(Leaf &leaf);
    };
}

#endif // DAG_H

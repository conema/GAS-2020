#ifndef Y_NODE_H
#define Y_NODE_H

#include <cg3/geometry/segment2.h>

namespace dag
{
    class y_node
    {
    public:
        y_node();
    private:
        const cg3::Segment2d segment;
    };
}

#endif // Y_NODE_H

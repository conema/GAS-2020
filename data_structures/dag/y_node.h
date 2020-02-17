#ifndef Y_NODE_H
#define Y_NODE_H

#include <cg3/geometry/segment2.h>
#include "node.h"

namespace dag
{
    class y_node : public Node
    {
    public:
        y_node(size_t &segmentId);
    private:
        const size_t segmentId;
    };
}

#endif // Y_NODE_H

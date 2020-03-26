#ifndef Y_NODE_H
#define Y_NODE_H

#include <cg3/geometry/segment2.h>
#include "node.h"

namespace dag
{
    class YNode : public Node
    {
    public:
        YNode(const size_t &segmentId);

        size_t getSegmentId() const;

    private:
        const size_t segmentId;
    };
}

#endif // Y_NODE_H

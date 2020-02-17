#ifndef X_NODE_H
#define X_NODE_H

#include <cg3/geometry/point2.h>
#include "node.h"

namespace dag
{
    class XNode: public Node
    {
    public:
        XNode(const size_t &pointId);


    private:
        const size_t pointId;
    };
}

#endif // X_NODE_H

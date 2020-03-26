#ifndef LEAF_H
#define LEAF_H

#include "node.h"

// Forward declaration to avoid circular includes
namespace tmap {
    class Trapezoid;
}

namespace dag
{
    class Leaf: public Node
    {
    public:
        Leaf(tmap::Trapezoid *trapezoid);

        tmap::Trapezoid* getTrapezoid();
        void setTrapezoid(tmap::Trapezoid* trapezoid);

    private:
        // Pointer to the trapezoid
        tmap::Trapezoid *trapezoid;
    };
}

#endif // LEAF_H

#ifndef LEAF_H
#define LEAF_H

// Forward declaration to avoid circular includes
namespace tmap {
    class Trapezoid;
}

namespace dag
{
    class Leaf
    {
    public:
        Leaf();
    private:
        // Pointer to the trapezoid
        const tmap::Trapezoid *trapezoid;
    };
}

#endif // LEAF_H

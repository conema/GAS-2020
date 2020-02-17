#include "leaf.h"

dag::Leaf::Leaf(tmap::Trapezoid *trapezoid):
    Node(dag::NodeType::LEAF),
    trapezoid(trapezoid)
{

}

tmap::Trapezoid* dag::Leaf::getTrapezoid()
{
    return trapezoid;
}

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

void dag::Leaf::setTrapezoid(tmap::Trapezoid* trapezoid)
{
    this->trapezoid = trapezoid;
}

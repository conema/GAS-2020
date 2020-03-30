#include "leaf.h"

dag::Leaf::Leaf(tmap::Trapezoid *trapezoid):
    Node(dag::NodeType::LEAF),
    trapezoid(trapezoid)
{

}

/**
 * @brief get the trapezoid associated to the leaf
 * @return a trapezoid
 */
tmap::Trapezoid* dag::Leaf::getTrapezoid()
{
    return trapezoid;
}

/**
 * @brief set the trapezoid associated to the leaf
 * @param trapezoid
 */
void dag::Leaf::setTrapezoid(tmap::Trapezoid* trapezoid)
{
    this->trapezoid = trapezoid;
}

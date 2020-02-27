#include "trapezoid.h"


tmap::Trapezoid::Trapezoid()
{

}

tmap::Trapezoid::Trapezoid(const size_t top,
                         const size_t bottom,
                         const size_t rightp,
                         const size_t leftp):
    top(top),
    bottom(bottom),
    rightp(rightp),
    leftp(leftp),
    color(generateColor()),
    leaf(nullptr)
{

}

/* ----- Overloading ----- */

bool tmap::Trapezoid::operator== (const Trapezoid& value){
    return (value.getTop() == getTop() &&
        value.getBottom() == getBottom() &&
        value.getRightp() == getRightp() &&
        value.getLeftp() == getLeftp());
}

/* ----- Getter/Setter ----- */

const size_t &tmap::Trapezoid::getTop() const
{
    return top;
}

const size_t &tmap::Trapezoid::getBottom() const
{
    return bottom;
}

const size_t &tmap::Trapezoid::getRightp() const
{
    return rightp;
}

void tmap::Trapezoid::setRightp(const size_t &value)
{
    rightp = value;
}

const size_t &tmap::Trapezoid::getLeftp() const
{
    return leftp;
}

tmap::Trapezoid* tmap::Trapezoid::getUpperRightTrapezoid() const
{
    return upperRightTrapezoid;
}

void tmap::Trapezoid::setUpperRightTrapezoid(Trapezoid* value)
{
    upperRightTrapezoid = value;
}

tmap::Trapezoid* tmap::Trapezoid::getUpperLeftTrapezoid() const
{
    return upperLeftTrapezoid;
}

void tmap::Trapezoid::setUpperLeftTrapezoid(Trapezoid* value)
{
    upperLeftTrapezoid = value;
}

tmap::Trapezoid* tmap::Trapezoid::getLowerLeftTrapezoid() const
{
    return lowerLeftTrapezoid;
}

void tmap::Trapezoid::setLowerLeftTrapezoid(tmap::Trapezoid* value)
{
    lowerLeftTrapezoid = value;
}

tmap::Trapezoid* tmap::Trapezoid::getLowerRightTrapezoid() const
{
    return lowerRightTrapezoid;
}

void tmap::Trapezoid::setLowerRightTrapezoid(tmap::Trapezoid* value)
{
    lowerRightTrapezoid = value;
}

/* ----- Other methods ----- */
void tmap::Trapezoid::updateAdjacencies(tmap::Trapezoid* lowerLeftTrapezoid,
                                        tmap::Trapezoid* upperLeftTrapezoid,
                                        tmap::Trapezoid* upperRightTrapezoid,
                                        tmap::Trapezoid* lowerRightTrapezoid){
    tmap::Trapezoid::setLowerLeftTrapezoid(lowerLeftTrapezoid);
    tmap::Trapezoid::setUpperLeftTrapezoid(upperLeftTrapezoid);
    tmap::Trapezoid::setUpperRightTrapezoid(upperRightTrapezoid);
    tmap::Trapezoid::setLowerRightTrapezoid(lowerRightTrapezoid);
}

const cg3::Color tmap::Trapezoid::getColor() const
{
    return color;
}

dag::Leaf *tmap::Trapezoid::getLeaf() const
{
    return leaf;
}

void tmap::Trapezoid::setLeaf(dag::Leaf *value)
{
    leaf = value;
}

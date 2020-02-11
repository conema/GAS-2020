#include "trapezoid.h"

tmap::Trapezoid::Trapezoid(const cg3::Segment2d top,
                         const cg3::Segment2d bottom,
                         const cg3::Point2d rightp,
                         const cg3::Point2d leftp):
    top(top),
    bottom(bottom),
    rightp(rightp),
    leftp(leftp),
    color(generateColor())
{

}

/* ----- Overloading ----- */

bool tmap::Trapezoid::operator== (const Trapezoid& value){
    if (value.getTop() == getTop() &&
        value.getBottom() == getBottom() &&
        value.getRightp() == getRightp() &&
        value.getLeftp() == getLeftp()) {

        return true;
    }

    return false;
}

/* ----- Getter/Setter ----- */

const cg3::Segment2d &tmap::Trapezoid::getTop() const
{
    return top;
}

const cg3::Segment2d &tmap::Trapezoid::getBottom() const
{
    return bottom;
}

const cg3::Point2d &tmap::Trapezoid::getRightp() const
{
    return rightp;
}

const cg3::Point2d &tmap::Trapezoid::getLeftp() const
{
    return leftp;
}

const tmap::Trapezoid *tmap::Trapezoid::getUpperRightTrapezoid() const
{
    return upperRightTrapezoid;
}

void tmap::Trapezoid::setUpperRightTrapezoid(const Trapezoid *value)
{
    upperRightTrapezoid = value;
}

const tmap::Trapezoid *tmap::Trapezoid::getUpperLeftTrapezoid() const
{
    return upperLeftTrapezoid;
}

void tmap::Trapezoid::setUpperLeftTrapezoid(const Trapezoid *value)
{
    upperLeftTrapezoid = value;
}

const tmap::Trapezoid *tmap::Trapezoid::getLowerLeftTrapezoid() const
{
    return lowerLeftTrapezoid;
}

void tmap::Trapezoid::setLowerLeftTrapezoid(const Trapezoid *value)
{
    lowerLeftTrapezoid = value;
}

const tmap::Trapezoid *tmap::Trapezoid::getLowerRightTrapezoid() const
{
    return lowerRightTrapezoid;
}

void tmap::Trapezoid::setLowerRightTrapezoid(const tmap::Trapezoid *value)
{
    lowerRightTrapezoid = value;
}

/* ----- Other methods ----- */
void tmap::Trapezoid::updateAdjacencies(const tmap::Trapezoid *lowerLeftTrapezoid,
                       const tmap::Trapezoid *upperLeftTrapezoid,
                       const tmap::Trapezoid *upperRightTrapezoid,
                       const tmap::Trapezoid *lowerRightTrapezoid){
    tmap::Trapezoid::setLowerLeftTrapezoid(lowerLeftTrapezoid);
    tmap::Trapezoid::setUpperLeftTrapezoid(upperLeftTrapezoid);
    tmap::Trapezoid::setUpperRightTrapezoid(upperRightTrapezoid);
    tmap::Trapezoid::setLowerRightTrapezoid(lowerRightTrapezoid);
}

cg3::Color tmap::Trapezoid::getColor() const
{
    return color;
}

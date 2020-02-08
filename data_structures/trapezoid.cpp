#include "trapezoid.h"

tg::Trapezoid::Trapezoid(const cg3::Segment2d top,
                         const cg3::Segment2d bottom,
                         const cg3::Point2d rightp,
                         const cg3::Point2d leftp) :
    top(top),
    bottom(bottom),
    rightp(rightp),
    leftp(leftp)
{

}

/* ----- Overloading ----- */

bool tg::Trapezoid::operator== (const Trapezoid& value){
    if (value.getTop() == getTop() &&
        value.getBottom() == getBottom() &&
        value.getRightp() == getRightp() &&
        value.getLeftp() == getLeftp()) {

        return true;
    }

    return false;
}

/* ----- Getter/Setter ----- */

const cg3::Segment2d &tg::Trapezoid::getTop() const
{
    return top;
}

const cg3::Segment2d &tg::Trapezoid::getBottom() const
{
    return bottom;
}

const cg3::Point2d &tg::Trapezoid::getRightp() const
{
    return rightp;
}

const cg3::Point2d &tg::Trapezoid::getLeftp() const
{
    return leftp;
}

const tg::Trapezoid *tg::Trapezoid::getUpperRightTrapezoid() const
{
    return upperRightTrapezoid;
}

void tg::Trapezoid::setUpperRightTrapezoid(const Trapezoid *value)
{
    upperRightTrapezoid = value;
}

const tg::Trapezoid *tg::Trapezoid::getUpperLeftTrapezoid() const
{
    return upperLeftTrapezoid;
}

void tg::Trapezoid::setUpperLeftTrapezoid(const Trapezoid *value)
{
    upperLeftTrapezoid = value;
}

const tg::Trapezoid *tg::Trapezoid::getLowerLeftTrapezoid() const
{
    return lowerLeftTrapezoid;
}

void tg::Trapezoid::setLowerLeftTrapezoid(const Trapezoid *value)
{
    lowerLeftTrapezoid = value;
}

const tg::Trapezoid *tg::Trapezoid::getLowerRightTrapezoid() const
{
    return lowerRightTrapezoid;
}

void tg::Trapezoid::setLowerRightTrapezoid(const tg::Trapezoid *value)
{
    lowerRightTrapezoid = value;
}

/* ----- Other methods ----- */
void tg::Trapezoid::updateAdjacencies(const tg::Trapezoid *lowerLeftTrapezoid,
                       const tg::Trapezoid *upperLeftTrapezoid,
                       const tg::Trapezoid *upperRightTrapezoid,
                       const tg::Trapezoid *lowerRightTrapezoid){
    tg::Trapezoid::setLowerLeftTrapezoid(lowerLeftTrapezoid);
    tg::Trapezoid::setUpperLeftTrapezoid(upperLeftTrapezoid);
    tg::Trapezoid::setUpperRightTrapezoid(upperRightTrapezoid);
    tg::Trapezoid::setLowerRightTrapezoid(lowerRightTrapezoid);
}

#include "trapezoid.h"


tmap::Trapezoid::Trapezoid()
{

}


tmap::Trapezoid::Trapezoid(Trapezoid const &trapezoid):
    top(trapezoid.getTop()),
    bottom(trapezoid.getBottom()),
    rightp(trapezoid.getRightp()),
    leftp(trapezoid.getLeftp()),
    color(trapezoid.getColor()),
    leaf(trapezoid.getLeaf()),
    upperRightTrapezoid(trapezoid.getUpperRightTrapezoid()),
    upperLeftTrapezoid(trapezoid.getUpperLeftTrapezoid()),
    lowerRightTrapezoid(trapezoid.getLowerRightTrapezoid()),
    lowerLeftTrapezoid(trapezoid.getLowerLeftTrapezoid())
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
    leaf(nullptr),
    upperRightTrapezoid(nullptr),
    upperLeftTrapezoid(nullptr),
    lowerRightTrapezoid(nullptr),
    lowerLeftTrapezoid(nullptr)
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
/**
 * @brief get the top segment of the trapezoid
 * @return the id of the top segment
 */
const size_t &tmap::Trapezoid::getTop() const
{
    return top;
}

/**
 * @brief get the bottom segment of the trapezoid
 * @return the id of the bottom segment
 */
const size_t &tmap::Trapezoid::getBottom() const
{
    return bottom;
}

/**
 * @brief get the right point of the trapezoid
 * @return the point id
 */
const size_t &tmap::Trapezoid::getRightp() const
{
    return rightp;
}

/**
 * @brief set the right point of the trapezoid
 * @param value
 */
void tmap::Trapezoid::setRightp(const size_t &value)
{
    rightp = value;
}

/**
 * @brief get the left point of the trapezoid
 * @return the point id
 */
const size_t &tmap::Trapezoid::getLeftp() const
{
    return leftp;
}

/**
 * @brief get the upper right adjacent trapezoid
 * @return a trapezoid
 */
tmap::Trapezoid* tmap::Trapezoid::getUpperRightTrapezoid() const
{
    return upperRightTrapezoid;
}

/**
 * @brief set the upper right trapezoid
 * @param value
 */
void tmap::Trapezoid::setUpperRightTrapezoid(Trapezoid* value)
{
    upperRightTrapezoid = value;
}

/**
 * @brief get the upper left adjacent trapezoid
 * @return a trapezoid
 */
tmap::Trapezoid* tmap::Trapezoid::getUpperLeftTrapezoid() const
{
    return upperLeftTrapezoid;
}

/**
 * @brief set the upper left trapezoid
 * @param value
 */
void tmap::Trapezoid::setUpperLeftTrapezoid(Trapezoid* value)
{
    upperLeftTrapezoid = value;
}

/**
 * @brief get the lower left adjacent trapezoid
 * @return a trapezoid
 */
tmap::Trapezoid* tmap::Trapezoid::getLowerLeftTrapezoid() const
{
    return lowerLeftTrapezoid;
}

/**
 * @brief set the lower left trapezoid
 * @param value
 */
void tmap::Trapezoid::setLowerLeftTrapezoid(tmap::Trapezoid* value)
{
    lowerLeftTrapezoid = value;
}

/**
 * @brief get the lower right adjacent trapezoid
 * @return a trapezoid
 */
tmap::Trapezoid* tmap::Trapezoid::getLowerRightTrapezoid() const
{
    return lowerRightTrapezoid;
}

/**
 * @brief set the lower right trapezoid
 * @param value
 */
void tmap::Trapezoid::setLowerRightTrapezoid(tmap::Trapezoid* value)
{
    lowerRightTrapezoid = value;
}

/* ----- Other methods ----- */
/**
 * @brief "Syntactic sugar" to update the adjacencies of a trapezoid
 * @param lowerLeftTrapezoid
 * @param upperLeftTrapezoid
 * @param upperRightTrapezoid
 * @param lowerRightTrapezoid
 */
void tmap::Trapezoid::updateAdjacencies(tmap::Trapezoid* lowerLeftTrapezoid,
                                        tmap::Trapezoid* upperLeftTrapezoid,
                                        tmap::Trapezoid* upperRightTrapezoid,
                                        tmap::Trapezoid* lowerRightTrapezoid){
    tmap::Trapezoid::setLowerLeftTrapezoid(lowerLeftTrapezoid);
    tmap::Trapezoid::setUpperLeftTrapezoid(upperLeftTrapezoid);
    tmap::Trapezoid::setUpperRightTrapezoid(upperRightTrapezoid);
    tmap::Trapezoid::setLowerRightTrapezoid(lowerRightTrapezoid);
}

/**
 * @brief get the color of the trapezoid
 * @return a color
 */
const cg3::Color tmap::Trapezoid::getColor() const
{
    return color;
}

/**
 * @brief get the #Leaf of the trapezoid
 * @return a leaf
 */
dag::Leaf *tmap::Trapezoid::getLeaf() const
{
    return leaf;
}

/**
 * @brief set the #Leaf of the trapezoid
 * @param value
 */
void tmap::Trapezoid::setLeaf(dag::Leaf *value)
{
    leaf = value;
}

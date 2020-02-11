#include "trapezoidalmap_helper.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tmap::insertSegment(const cg3::Segment2d &segment)
{
    // TODO: da cambiare
    tmap::Trapezoid oldTrapezoid = tmap::TrapezoidalMap::firstTrapezoid();

    // p1 leftmost and p2 rightmost
    const cg3::Point2d &leftMost = segment.p1().min(segment.p2());
    const cg3::Point2d &rightMost = segment.p1().max(segment.p2());;

    // For new trapezoid for the new segment
    tmap::Trapezoid A(oldTrapezoid.getTop(),
                      oldTrapezoid.getBottom(),
                      leftMost,
                      oldTrapezoid.getLeftp());

    tmap::Trapezoid B(oldTrapezoid.getTop(),
                      oldTrapezoid.getBottom(),
                      oldTrapezoid.getRightp(),
                      rightMost);

    tmap::Trapezoid C(oldTrapezoid.getTop(),
                      segment,
                      rightMost,
                      leftMost);

    tmap::Trapezoid D(segment,
                      oldTrapezoid.getBottom(),
                      rightMost,
                      leftMost);

    // Add adjacencies
    A.Trapezoid::updateAdjacencies(oldTrapezoid.getLowerLeftTrapezoid(),
                                   oldTrapezoid.getUpperLeftTrapezoid(),
                                   &C,
                                   &D);

    B.Trapezoid::updateAdjacencies(&D,
                                   &C,
                                   oldTrapezoid.getUpperRightTrapezoid(),
                                   oldTrapezoid.getLowerRightTrapezoid());
    C.Trapezoid::updateAdjacencies(&A,
                                   &A,
                                   &B,
                                   &B);
    D.Trapezoid::updateAdjacencies(&A,
                                   &A,
                                   &B,
                                   &B);

    tmap::TrapezoidalMap::removeTrapezoid(oldTrapezoid);
    tmap::TrapezoidalMap::addTrapezoid(A);
    tmap::TrapezoidalMap::addTrapezoid(B);
    tmap::TrapezoidalMap::addTrapezoid(C);
    tmap::TrapezoidalMap::addTrapezoid(D);
}

/**
 * @brief Get the intersection point between one segment and one straight line
 * @param[in] s1: first segment
 * @param[in] s2: second segment
 * @return intersection point
 */
cg3::Point2d tmap::findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p)
{
    long double rect = ((s.p2().x() - p.x())) * (s.p1().y()) + ((p.x() - s.p1().x())) * (s.p2().y());
    double y = rect / (s.p2().x() - s.p1().x());

    return cg3::Point2d(p.x(), y);
}

/**
 * @brief Generate a random pastel color
 * @param[in] startColor
 * @return random color
 */
cg3::Color tmap::generateColor(const cg3::Color &startColor){
    int red = rand()%256;
    int green = rand()%256;
    int blue = rand()%256;

    // average the colors
    red = (red + startColor.red()) / 2;
    green = (green + startColor.green()) / 2;
    blue = (blue + startColor.blue()) / 2;

    return cg3::Color(red, green, blue);
}


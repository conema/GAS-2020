#include "trapezoidalmap.h"

std::list<tmap::Trapezoid> tmap::TrapezoidalMap::trapezoids = [] {
    std::list<tmap::Trapezoid> trapezoids;

    return trapezoids;
}();

tmap::TrapezoidalMap::TrapezoidalMap() :
    boundingBox(cg3::Point2d(0,0), cg3::Point2d(0,0))
{
}

tmap::TrapezoidalMap::TrapezoidalMap(const int &boundingbox) :
    boundingBox(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox,boundingbox)){

    // Initialize Trapezoidal Map with the boundingbox trapezoid (S0)
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox, boundingbox), cg3::Point2d(boundingbox, boundingbox));
    cg3::Segment2d bottom = cg3::Segment2d(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox, -boundingbox));
    cg3::Point2d leftp = cg3::Point2d(-boundingbox, -boundingbox);
    cg3::Point2d rightp = cg3::Point2d(boundingbox, boundingbox);

    tmap::Trapezoid S0(top,
                     bottom,
                     rightp,
                     leftp);
    S0.Trapezoid::updateAdjacencies(nullptr, nullptr, nullptr, nullptr);

    TrapezoidalMap::addTrapezoid(S0);
}

void tmap::TrapezoidalMap::addTrapezoid(const tmap::Trapezoid trapezoid)
{
    trapezoids.push_back(trapezoid);
}

void tmap::TrapezoidalMap::removeTrapezoid(const tmap::Trapezoid &trapezoid) {
    trapezoids.remove(trapezoid);
}

const tmap::Trapezoid& tmap::TrapezoidalMap::firstTrapezoid()
{
    return trapezoids.front();
}

const cg3::BoundingBox2& tmap::TrapezoidalMap::getBoundingBox() const
{
    return boundingBox;
}

const tmap::TrapezoidalMap::Trapezoids &tmap::TrapezoidalMap::getTrapezoids()
{
    return trapezoids;
}




#include "trapezoidalmap.h"
#include "trapezoidalmap.h"

TrapezoidalMap::TrapezoidalMap() :
    boundingBox(cg3::Point2d(0,0), cg3::Point2d(0,0))
{

}

TrapezoidalMap::TrapezoidalMap(const int &boundingbox) :
    boundingBox(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox,boundingbox)) {
    // Initialize Trapezoidal Map with the boundingbox trapezoid (S0)
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox, boundingbox), cg3::Point2d(boundingbox, boundingbox));
    cg3::Segment2d bottom = cg3::Segment2d(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox, -boundingbox));
    cg3::Point2d leftp = cg3::Point2d(-boundingbox, -boundingbox);
    cg3::Point2d rightp = cg3::Point2d(boundingbox, boundingbox);

    tg::Trapezoid S0(top,
                     bottom,
                     rightp,
                     leftp);
    S0.Trapezoid::updateAdjacencies(nullptr, nullptr, nullptr, nullptr);

    TrapezoidalMap::addTrapezoid(S0);
}

void TrapezoidalMap::addTrapezoid(const tg::Trapezoid &trapezoid)
{
    trapezoids.push_back(trapezoid);
}

void TrapezoidalMap::addSegments(const cg3::Segment2d &segment)
{
/*
    boundingBox.setMax(cg3::Point2d(
            std::max(std::max(segment.p1().x(), segment.p2().x()), boundingBox.max().x()),
            std::max(std::max(segment.p1().y(), segment.p2().y()), boundingBox.max().y())));
    boundingBox.setMin(cg3::Point2d(
            std::min(std::min(segment.p1().x(), segment.p2().x()), boundingBox.min().x()),
            std::min(std::min(segment.p1().y(), segment.p2().y()), boundingBox.min().y())));

*/
    if (trapezoids.size() == 1){

        // TODO: da cambiare
       tg::Trapezoid oldTrapezoid = trapezoids.front();

        // For new trapezoid for the new segment
        tg::Trapezoid A(oldTrapezoid.getTop(),
                        oldTrapezoid.getBottom(),
                        segment.p1(),
                        oldTrapezoid.getLeftp());

        tg::Trapezoid B(oldTrapezoid.getTop(),
                        oldTrapezoid.getBottom(),
                        oldTrapezoid.getRightp(),
                        segment.p2());

        tg::Trapezoid C(oldTrapezoid.getTop(),
                        segment,
                        segment.p2(),
                        segment.p1());

        tg::Trapezoid D(segment,
                        oldTrapezoid.getBottom(),
                        segment.p2(),
                        segment.p1());

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

        trapezoids.remove(oldTrapezoid);
        trapezoids.push_back(A);
        trapezoids.push_back(B);
        trapezoids.push_back(C);
        trapezoids.push_back(D);

        //std::cout<<trapezoids.size()<<std::endl;

    }
}

const cg3::BoundingBox2& TrapezoidalMap::getBoundingBox() const
{
    return boundingBox;
}

const TrapezoidalMap::Trapezoids &TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}




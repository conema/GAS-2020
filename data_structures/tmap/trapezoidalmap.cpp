#include "trapezoidalmap.h"


tmap::TrapezoidalMap::TrapezoidalMap(const int &boundingbox, TrapezoidalMapDataset &trapezoidalMapDataset):
    boundingBox(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox,boundingbox)),
    trapezoidalMapDataset(trapezoidalMapDataset)
{

    cg3::Point2d leftp = cg3::Point2d(-boundingbox, -boundingbox);
    cg3::Point2d rightp = cg3::Point2d(boundingbox, boundingbox);

    // Initialize Trapezoidal Map with the boundingbox trapezoid (S0), but they can't have the same x coordinates
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox-0.01, boundingbox), rightp);
    cg3::Segment2d bottom = cg3::Segment2d(leftp, cg3::Point2d(boundingbox-0.01, -boundingbox));

    bool success;

    trapezoidalMapDataset.addSegment(top, success);
    trapezoidalMapDataset.addSegment(bottom, success);

    size_t id_top = trapezoidalMapDataset.findSegment(top, success);
    size_t id_bottom = trapezoidalMapDataset.findSegment(bottom, success);

    size_t id_leftp = trapezoidalMapDataset.findPoint(leftp, success);
    size_t id_rightp = trapezoidalMapDataset.findPoint(rightp, success);

    tmap::Trapezoid *S0 = new tmap::Trapezoid(id_top,
                     id_bottom,
                     id_rightp,
                     id_leftp);

    S0->Trapezoid::updateAdjacencies(nullptr, nullptr, nullptr, nullptr);


    addTrapezoid(S0);
}

tmap::TrapezoidalMap::~TrapezoidalMap()
{
    for (const auto& trapezoid: trapezoids) {
        delete trapezoid;
    }
}

void tmap::TrapezoidalMap::addTrapezoid(tmap::Trapezoid *trapezoid)
{
    trapezoids.insert(trapezoid);
}

void tmap::TrapezoidalMap::removeTrapezoid(tmap::Trapezoid *trapezoid)
{  
    trapezoids.erase(trapezoid);
    delete trapezoid;
}

const cg3::BoundingBox2& tmap::TrapezoidalMap::getBoundingBox() const
{
    return boundingBox;
}

const tmap::TrapezoidalMap::Trapezoids &tmap::TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tmap::TrapezoidalMap::insertSegment(const cg3::Segment2d &segment)
{
    // TODO: da cambiare
    tmap::Trapezoid *oldTrapezoid = *trapezoids.begin();

    // p1 leftmost and p2 rightmost
    const cg3::Point2d &leftMost = segment.p1().min(segment.p2());
    const cg3::Point2d &rightMost = segment.p1().max(segment.p2());;

    bool success;

    const size_t leftMostId = trapezoidalMapDataset.findPoint(leftMost, success);
    const size_t rightMostId = trapezoidalMapDataset.findPoint(rightMost, success);
    const size_t segmentId = trapezoidalMapDataset.findSegment(segment, success);

    // For new trapezoid for the new segment
    tmap::Trapezoid *A = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      oldTrapezoid->getBottom(),
                      leftMostId,
                      oldTrapezoid->getLeftp());

    tmap::Trapezoid *B = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      oldTrapezoid->getBottom(),
                      oldTrapezoid->getRightp(),
                      rightMostId);

    tmap::Trapezoid *C = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      segmentId,
                      rightMostId,
                      leftMostId);

    tmap::Trapezoid *D = new tmap::Trapezoid(segmentId,
                      oldTrapezoid->getBottom(),
                      rightMostId,
                      leftMostId);

    // Add adjacencies
    A->Trapezoid::updateAdjacencies(oldTrapezoid->getLowerLeftTrapezoid(),
                                   oldTrapezoid->getUpperLeftTrapezoid(),
                                   C,
                                   D);

    B->Trapezoid::updateAdjacencies(D,
                                   C,
                                   oldTrapezoid->getUpperRightTrapezoid(),
                                   oldTrapezoid->getLowerRightTrapezoid());
    C->Trapezoid::updateAdjacencies(A,
                                   A,
                                   B,
                                   B);
    D->Trapezoid::updateAdjacencies(A,
                                   A,
                                   B,
                                   B);

    removeTrapezoid(oldTrapezoid);
    addTrapezoid(A);
    addTrapezoid(B);
    addTrapezoid(C);
    addTrapezoid(D);
}


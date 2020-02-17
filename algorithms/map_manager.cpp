#include "map_manager.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tmap::buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset)
{
    // TODO: da cambiare
    tmap::Trapezoid *oldTrapezoid = *trapezoidalMap.getTrapezoids().begin();

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

    trapezoidalMap.removeTrapezoid(oldTrapezoid);
    trapezoidalMap.addTrapezoid(A);
    trapezoidalMap.addTrapezoid(B);
    trapezoidalMap.addTrapezoid(C);
    trapezoidalMap.addTrapezoid(D);
}

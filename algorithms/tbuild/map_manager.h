#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <data_structures/tmap/trapezoidalmap.h>
#include <algorithms/tmap/trapezoidalmap_helper.h>
#include <data_structures/dag/dag.h>
#include <data_structures/dag/node.h>
#include <data_structures/dag/x_node.h>
#include <data_structures/dag/y_node.h>
#include <data_structures/dag/leaf.h>

namespace tbuild {
    void buildMap(const cg3::Segment2d &segment,
                  tmap::TrapezoidalMap &trapezoidalMap,
                  dag::Dag &Dag);

    void oneTrapezoidIntersected(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &leftMostId, const size_t &rightMostId, tmap::Trapezoid * const &trapezoid);

    void multiTrapezoidIntersectedFirstEndpoint(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &rightMostId, const tmap::Trapezoid* const &trapezoidFirstEndpoint, tmap::Trapezoid*&oldUpper, tmap::Trapezoid* &oldLower);
    void multiTrapezoidIntersectedMiddle(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, tmap::Trapezoid* const &currentTrapezoid, tmap::Trapezoid* &oldUpper, tmap::Trapezoid* &oldLower );
    void multiTrapezoidIntersectedLastEndpoint(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &rightMostId, const tmap::Trapezoid* const &trapezoidSecondEndpoint, tmap::Trapezoid* &oldUpper, tmap::Trapezoid* &oldLower);

    std::vector<tmap::Trapezoid*> followSegment(const dag::Dag &dag,
                                                const cg3::Segment2d &segment,
                                                const tmap::TrapezoidalMap &trapezoidalMap);
}

#endif // MAP_MANAGER_H

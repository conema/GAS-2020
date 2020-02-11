#ifndef FOLLOW_SEGMENTS_H
#define FOLLOW_SEGMENTS_H

#include <data_structures/tmap/trapezoid.h>
#include <data_structures/tmap/trapezoidalmap.h>
#include <data_structures/dag/dag.h>
#include <cg3/geometry/segment2.h>

std::list<tmap::Trapezoid> followSegment(tmap::TrapezoidalMap &trapezoidalMap,
                                         dag::dag &dag,
                                         cg3::Segment2d &segment);

#endif // FOLLOW_SEGMENTS_H

#ifndef TRAPEZOIDALMAP_HELPER_H
#define TRAPEZOIDALMAP_HELPER_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include <cg3/utilities/color.h>
#include <data_structures/tmap/trapezoidalmap.h>

namespace tmap{
    class Trapezoid;

    void insertSegment(const cg3::Segment2d &segment);
    cg3::Point2d findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p);
    cg3::Color generateColor(const cg3::Color &startColor = cg3::Color(255, 255, 255));
}

#endif // TRAPEZOIDALMAP_HELPER_H

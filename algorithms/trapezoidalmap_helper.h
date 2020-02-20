#ifndef TRAPEZOIDALMAP_HELPER_H
#define TRAPEZOIDALMAP_HELPER_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include <cg3/utilities/color.h>

namespace tmap{
    class Trapezoid;

    cg3::Point2d findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p);
    cg3::Color generateColor(const cg3::Color &startColor = cg3::Color(255, 255, 255));
    bool findPointSide(const cg3::Segment2d &s, const cg3::Point2d &p);
    bool findSlope(const cg3::Point2d &p1, const cg3::Point2d &p2);
    void orderPoint(cg3::Point2d &p1, cg3::Point2d &p2);
}

#endif // TRAPEZOIDALMAP_HELPER_H

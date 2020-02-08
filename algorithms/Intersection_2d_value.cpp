#include "Intersection_2d_value.h"

/**
 * @brief Get the intersection point between one segment and one straight line
 * @param[in] s1: first segment
 * @param[in] s2: second segment
 * @return intersection point
 */
cg3::Point2d findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p)
{
    long double rect = ((s.p2().x() - p.x())) * (s.p1().y()) + ((p.x() - s.p1().x())) * (s.p2().y());
    double y = rect / (s.p2().x() - s.p1().x());

    return cg3::Point2d(p.x(), y);
}

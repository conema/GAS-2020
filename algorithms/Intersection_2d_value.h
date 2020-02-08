#ifndef INTERSECTION2DVALUE_H
#define INTERSECTION2DVALUE_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point_t;
typedef boost::geometry::model::segment<point_t> segment_t;

cg3::Point2d findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p);


#endif // INTERSECTION2DVALUE_H

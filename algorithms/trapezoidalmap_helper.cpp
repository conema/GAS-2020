#include "trapezoidalmap_helper.h"

/**
 * @brief Get the intersection point between one segment and one straight line
 * @param[in] s1: first segment
 * @param[in] s2: second segment
 * @return intersection point
 */
cg3::Point2d tmap::findIntersectionPoint(const cg3::Segment2d &s, const cg3::Point2d &p)
{
    long double rect = ((s.p2().x() - p.x())) * (s.p1().y()) + ((p.x() - s.p1().x())) * (s.p2().y());
    double y = rect / (s.p2().x() - s.p1().x());

    return cg3::Point2d(p.x(), y);
}

/**
 * @brief Generate a random pastel color
 * @param[in] startColor: base color to start generation
 * @return random color
 */
cg3::Color tmap::generateColor(const cg3::Color &startColor){
    int red = rand()%256;
    int green = rand()%256;
    int blue = rand()%256;

    // average the colors
    red = (red + startColor.red()) / 2;
    green = (green + startColor.green()) / 2;
    blue = (blue + startColor.blue()) / 2;

    return cg3::Color(red, green, blue);
}

/**
 * @brief Find if a points lies on the left or on the right of a segment
 * @param[in] s: the segment
 * @param[in] p: the point
 * @return true if point is at the left, otherwise false
 */
bool tmap::findPointSide(const cg3::Segment2d &s, const cg3::Point2d &p)
{
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();

    tmap::orderPoint(p1, p2);

    return (p2.x() - p1.x()) * (p.y() - p1.y()) - (p2.y() - p1.y()) * (p.x() - p1.x()) > 0;
}

/**
 * @brief Return true if the slope of a segment is positive
 * @param[in] p1
 * @param[in] p2
 * @return true if the the slope is positive, else otherwise
 */
bool tmap::findSlope(const cg3::Point2d &p1, const cg3::Point2d &p2)
{
    //return (((p2.y()-p1.y())/(p2.x()-p1.x())) > 0);
    return p2.x()>p1.x();
}

/**
 * @brief Order two point
 * @param[in] p1
 * @param[in] p2
 * @return p1 will contain the leftmost point and p2 will containt the righmost
 */
void tmap::orderPoint(cg3::Point2d &p1, cg3::Point2d &p2)
{
    if (!findSlope(p1, p2)){
        std::swap(p2, p1);
   }
}

/**
 * @brief return the leftmost point of a segment
 * @param[in] segment
 * @return the leftmost point
 */
cg3::Point2d tmap::leftMostPoint(cg3::Segment2d segment)
{
    if (segment.p1().x() > segment.p2().x()){
        return segment.p2();
    }

    return segment.p1();
}

/**
 * @brief Given two segments, return the first different points of segment1 between segment1 and segment2
 * @param[in] segment1 and segment2
 * @return the first different point
 */
cg3::Point2d tmap::differentPoint(const cg3::Segment2d &segment1, const cg3::Segment2d &segment2)
{
    if (segment1.p1() == segment2.p1() || segment1.p1() == segment2.p2()){
        return segment1.p2();
    } else if (segment1.p2() == segment2.p1() || segment1.p2() == segment2.p2()){
        return segment1.p1();
    }

    return segment1.p1();
}

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
    return (s.p2().x() - s.p1().x()) * (p.y() - s.p1().y()) - (s.p2().y() - s.p1().y()) * (p.x() - s.p1().x()) > 0;
}

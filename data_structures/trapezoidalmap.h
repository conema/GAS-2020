#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <data_structures/trapezoid.h>
#include <cg3/geometry/bounding_box2.h>

class TrapezoidalMap
{
public:
    typedef std::list<tg::Trapezoid> Trapezoids;
    TrapezoidalMap();
    TrapezoidalMap(const int &boundingbox);
    void addTrapezoid(const tg::Trapezoid &trapezoid);
    void addSegments(const cg3::Segment2d &segment);

    const cg3::BoundingBox2& getBoundingBox() const;
    const Trapezoids& getTrapezoids() const;

private:
    Trapezoids trapezoids;

    cg3::BoundingBox2 boundingBox;
};

#endif // TRAPEZOIDALMAP_H

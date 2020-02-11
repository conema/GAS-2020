#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <data_structures/tmap/trapezoid.h>
#include <cg3/geometry/bounding_box2.h>

namespace tmap
{
    class TrapezoidalMap
    {
    public:
        typedef std::list<tmap::Trapezoid> Trapezoids;

        TrapezoidalMap();
        TrapezoidalMap(const int &boundingbox);

        static void addTrapezoid(const tmap::Trapezoid trapezoid);
        static void removeTrapezoid(const tmap::Trapezoid &trapezoid);
        static const tmap::Trapezoid& firstTrapezoid();

        const cg3::BoundingBox2& getBoundingBox() const;
        static const Trapezoids& getTrapezoids();

    private:
        static Trapezoids trapezoids;

        cg3::BoundingBox2 boundingBox;
    };
}

#endif // TRAPEZOIDALMAP_H

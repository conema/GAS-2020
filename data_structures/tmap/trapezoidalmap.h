#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <data_structures/tmap/trapezoid.h>
#include <cg3/geometry/bounding_box2.h>
#include <data_structures/trapezoidalmap_dataset.h>

namespace tmap
{
    class TrapezoidalMap
    {
    public:
        typedef std::unordered_set<tmap::Trapezoid*> Trapezoids;

        TrapezoidalMap(const int &boundingbox, TrapezoidalMapDataset &trapezoidalMapDataset);
        TrapezoidalMap(const TrapezoidalMap&) = delete;
        TrapezoidalMap& operator=(const TrapezoidalMap&) = delete;
        ~TrapezoidalMap();

        void addTrapezoid(tmap::Trapezoid *trapezoid);
        void removeTrapezoid(tmap::Trapezoid *trapezoid);
        const tmap::Trapezoid& firstTrapezoid() const;

        const cg3::BoundingBox2& getBoundingBox() const;
        const Trapezoids& getTrapezoids() const;
    private:
        // To store all the trapezoids
        Trapezoids trapezoids;

        cg3::BoundingBox2 boundingBox;
    protected:
        // To get a reference to points/segments
        TrapezoidalMapDataset &trapezoidalMapDataset;
    };
}

#endif // TRAPEZOIDALMAP_H

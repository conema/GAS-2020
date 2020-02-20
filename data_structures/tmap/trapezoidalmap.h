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

        TrapezoidalMap(const int &boundingbox);
        //TrapezoidalMap(const TrapezoidalMap&) = delete;
        //TrapezoidalMap& operator=(const TrapezoidalMap&) = delete;
        ~TrapezoidalMap();

        void initializeTrapezoids(TrapezoidalMapDataset &trapezoidalMapDataset);

        void addTrapezoid(tmap::Trapezoid *trapezoid);
        void removeTrapezoid(tmap::Trapezoid *trapezoid);
        const tmap::Trapezoid& firstTrapezoid() const;

        const cg3::BoundingBox2& getBoundingBox() const;
        const Trapezoids& getTrapezoids() const;

        void removeTrapezoids();
    private:
        // To store all the trapezoids
        Trapezoids trapezoids;

        // The trapezoidmap occupy all the external boundingbox
        cg3::BoundingBox2 boundingBox;
    };
}

#endif // TRAPEZOIDALMAP_H

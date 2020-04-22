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
        typedef std::pair<size_t, size_t> IndexedSegment2d;

        TrapezoidalMap(const int &boundingbox);
        TrapezoidalMap(TrapezoidalMap&& in);
        TrapezoidalMap& operator = (TrapezoidalMap&& in);

        TrapezoidalMap(const TrapezoidalMap& of);
        TrapezoidalMap& operator=(TrapezoidalMap other);

        ~TrapezoidalMap();

        void initializeTrapezoids();

        // Trapezoids Methods
        void addTrapezoid(tmap::Trapezoid *trapezoid);
        void removeTrapezoid(Trapezoid *trapezoid);
        void removeTrapezoids();
        const tmap::Trapezoid& firstTrapezoid() const;

        // Boundingbox Mathods
        const cg3::BoundingBox2& getBoundingBox() const;
        const Trapezoids& getTrapezoids() const;


        // Segment and points method
        size_t addPoint(const cg3::Point2d& point, bool& pointInserted);
        size_t addSegment(const cg3::Segment2d& segment, bool& segmentInserted);

        size_t findPoint(const cg3::Point2d& point, bool& found);
        size_t findSegment(const cg3::Segment2d& segment, bool& found);
        size_t findIndexedSegment(const IndexedSegment2d &indexedSegment, bool &found);

        const cg3::Point2d& getPoint(size_t id) const;
        cg3::Segment2d getSegment(size_t id) const;
    private:
        // To store the trapezoids
        Trapezoids trapezoids;

        // The trapezoidmap occupy all the external boundingbox
        cg3::BoundingBox2 boundingBox;
        std::unordered_map<cg3::Point2d, size_t> pointMap;
        std::unordered_map<IndexedSegment2d, size_t> segmentMap;

        std::vector<cg3::Point2d> points;
        std::vector<IndexedSegment2d> indexedSegments;
        void swap(TrapezoidalMap &other);
    };
}

#endif // TRAPEZOIDALMAP_H

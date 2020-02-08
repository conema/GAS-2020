#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

/**
 * @brief This class represent a trapezoid in the trapezoid geometry (td) namespace
 */

namespace tg
{
    class Trapezoid
    {
    public:
        Trapezoid(const cg3::Segment2d& top,
                  const cg3::Segment2d& bottom,
                  const cg3::Point2d& rightp,
                  const cg3::Point2d& leftp);

        bool operator== (const Trapezoid &value);

        const cg3::Segment2d &getTop() const;
        void setTop(const cg3::Segment2d &value);

        const cg3::Segment2d &getBottom() const;
        void setBottom(const cg3::Segment2d &value);

        const cg3::Point2d &getRightp() const;
        void setRightp(const cg3::Point2d &value);

        const cg3::Point2d &getLeftp() const;
        void setLeftp(const cg3::Point2d &value);

        const Trapezoid *getUpperRightTrapezoid() const;
        void setUpperRightTrapezoid(const Trapezoid *value);

        const Trapezoid *getUpperLeftTrapezoid() const;
        void setUpperLeftTrapezoid(const Trapezoid *value);

        const Trapezoid *getLowerRightTrapezoid() const;
        void setLowerRightTrapezoid(const Trapezoid *value);

        const Trapezoid *getLowerLeftTrapezoid() const;
        void setLowerLeftTrapezoid(const Trapezoid *value);

        void updateAdjacencies(const Trapezoid *lowerLeftTrapezoid,
                               const Trapezoid *upperLeftTrapezoid,
                               const Trapezoid *upperRightTrapezoid,
                               const Trapezoid *lowerRightTrapezoid);

    private:

        /* ----- Private fields ----- */

        // Edges bounding the trapezoid
        const cg3::Segment2d &top;
        const cg3::Segment2d &bottom;

        // Edges points
        const cg3::Point2d &rightp;
        const cg3::Point2d &leftp;

        /*
         * Pointers because we don't know them when I initialize the trapezoids
         */
        // Upper adjacent trapezoids
        const Trapezoid *upperRightTrapezoid;
        const Trapezoid *upperLeftTrapezoid;

        // Lower adjacent trapezoids
        const Trapezoid *lowerRightTrapezoid;
        const Trapezoid *lowerLeftTrapezoid;
    };
}

#endif // TRAPEZOID_H

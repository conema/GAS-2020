#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <cg3/utilities/color.h>
#include <data_structures/dag/leaf.h>
#include <algorithms/trapezoidalmap_helper.h>

#define YESDEBUG false

/**
 * @brief This class represent a trapezoid
 */

namespace tmap
{
    class Trapezoid
    {
    public:
        Trapezoid();
        Trapezoid(const size_t top,
                  const size_t bottom,
                  const size_t rightp,
                  const size_t leftp);

        bool operator== (const Trapezoid &value);

        const size_t &getTop() const;
        void setTop(const size_t &value);

        const size_t &getBottom() const;
        void setBottom(const size_t &value);

        const size_t &getRightp() const;
        void setRightp(const size_t &value);

        const size_t &getLeftp() const;
        void setLeftp(const size_t &value);

        Trapezoid* getUpperRightTrapezoid() const;
        void setUpperRightTrapezoid(Trapezoid* value);

        Trapezoid* getUpperLeftTrapezoid() const;
        void setUpperLeftTrapezoid(Trapezoid* value);

        Trapezoid* getLowerRightTrapezoid() const;
        void setLowerRightTrapezoid(Trapezoid* value);

        Trapezoid* getLowerLeftTrapezoid() const;
        void setLowerLeftTrapezoid(Trapezoid* value);

        void updateAdjacencies(Trapezoid* lowerLeftTrapezoid,
                               Trapezoid* upperLeftTrapezoid,
                               Trapezoid* upperRightTrapezoid,
                               Trapezoid* lowerRightTrapezoid);

        const cg3::Color getColor() const;

        dag::Leaf *getLeaf() const;
        void setLeaf(dag::Leaf *value);

    private:

        /* ----- Private fields ----- */

        // Edges bounding the trapezoid
        size_t top;
        size_t bottom;

        // Edges points
        size_t rightp;
        size_t leftp;

        // Color
        const cg3::Color color;

        // Pointer to DAG leaf
        dag::Leaf *leaf;

        // Upper adjacent trapezoids
        Trapezoid *upperRightTrapezoid;
        Trapezoid *upperLeftTrapezoid;

        // Lower adjacent trapezoids
        Trapezoid *lowerRightTrapezoid;
        Trapezoid *lowerLeftTrapezoid;
    };
}

#endif // TRAPEZOID_H

#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include <data_structures/trapezoidalmap.h>

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/utilities/color.h>

#include <algorithms/Intersection_2d_value.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap();
    DrawableTrapezoidalMap(const int &boundingbox);

    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;
    void draw() const;
};

#endif // DRAWABLETRAPEZOIDALMAP_H

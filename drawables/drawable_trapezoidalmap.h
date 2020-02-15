#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include <data_structures/tmap/trapezoidalmap.h>

#include <cg3/viewer/interfaces/drawable_object.h>

#include <algorithms/trapezoidalmap_helper.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class DrawableTrapezoidalMap : public tmap::TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap(const int &boundingbox, TrapezoidalMapDataset &trapezoidalMapDataset);

    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;
    void draw() const;
};

#endif // DRAWABLETRAPEZOIDALMAP_H

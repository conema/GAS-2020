#ifndef DRAWABLETRAPEZOIDALMAP_H
#define DRAWABLETRAPEZOIDALMAP_H

#include <data_structures/tmap/trapezoidalmap.h>

#include <cg3/viewer/interfaces/drawable_object.h>

#include <utils/tmaputils/tmaputils.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class DrawableTrapezoidalMap : public tmap::TrapezoidalMap, public cg3::DrawableObject
{
public:
    DrawableTrapezoidalMap(const int &boundingbox);

    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;
    void draw() const;

    tmap::Trapezoid *getHighlightedTrapezoid() const;
    void setHighlightedTrapezoid(tmap::Trapezoid *value);

private:
    tmap::Trapezoid* highlightedTrapezoid;
};

#endif // DRAWABLETRAPEZOIDALMAP_H

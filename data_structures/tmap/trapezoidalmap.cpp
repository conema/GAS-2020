#include "trapezoidalmap.h"


tmap::TrapezoidalMap::TrapezoidalMap(const int &boundingbox):
    boundingBox(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox,boundingbox))
{


}

tmap::TrapezoidalMap::~TrapezoidalMap()
{
    removeTrapezoids();
}

void tmap::TrapezoidalMap::initializeTrapezoids(TrapezoidalMapDataset &trapezoidalMapDataset)
{
    const int &boundingbox = getBoundingBox().lengthX()/2;

    cg3::Point2d leftp = cg3::Point2d(-boundingbox, -boundingbox);
    cg3::Point2d rightp = cg3::Point2d(boundingbox, boundingbox);

    // Initialize Trapezoidal Map with the boundingbox trapezoid (S0), but they can't have the same x coordinates
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox-0.01, boundingbox), rightp);
    cg3::Segment2d bottom = cg3::Segment2d(leftp, cg3::Point2d(boundingbox-0.01, -boundingbox));

    bool success;

    trapezoidalMapDataset.addSegment(top, success);
    trapezoidalMapDataset.addSegment(bottom, success);

    size_t id_top = trapezoidalMapDataset.findSegment(top, success);
    size_t id_bottom = trapezoidalMapDataset.findSegment(bottom, success);

    size_t id_leftp = trapezoidalMapDataset.findPoint(leftp, success);
    size_t id_rightp = trapezoidalMapDataset.findPoint(rightp, success);

    tmap::Trapezoid *S0 = new tmap::Trapezoid(id_top,
                     id_bottom,
                     id_rightp,
                     id_leftp);

    S0->Trapezoid::updateAdjacencies(nullptr, nullptr, nullptr, nullptr);


    addTrapezoid(S0);
}

void tmap::TrapezoidalMap::addTrapezoid(tmap::Trapezoid *trapezoid)
{
    trapezoids.insert(trapezoid);
}

void tmap::TrapezoidalMap::removeTrapezoid(tmap::Trapezoid *trapezoid)
{  
    trapezoids.erase(trapezoid);
    delete trapezoid;
}

const cg3::BoundingBox2& tmap::TrapezoidalMap::getBoundingBox() const
{
    return boundingBox;
}

const tmap::TrapezoidalMap::Trapezoids &tmap::TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}

void tmap::TrapezoidalMap::removeTrapezoids()
{
    for (const auto& trapezoid: trapezoids) {
        delete trapezoid;
    }

    trapezoids.clear();
}

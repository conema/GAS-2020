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
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox-((double)1/boundingbox), boundingbox), rightp);
    cg3::Segment2d bottom = cg3::Segment2d(leftp, cg3::Point2d(boundingbox+((double)1/boundingbox), -boundingbox));

    bool success;

    trapezoidalMapDataset.addSegment(top, success);
    assert(success);

    trapezoidalMapDataset.addSegment(bottom, success);
    assert(success);

    size_t id_top = trapezoidalMapDataset.findSegment(top, success);
    assert(success);

    size_t id_bottom = trapezoidalMapDataset.findSegment(bottom, success);
    assert(success);

    size_t id_leftp = trapezoidalMapDataset.findPoint(leftp, success);
    assert(success);

    size_t id_rightp = trapezoidalMapDataset.findPoint(rightp, success);
    assert(success);

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
    if (trapezoid == nullptr){
        return;
    }

    if (trapezoid->getLeaf() != nullptr) {
        trapezoid->getLeaf()->setTrapezoid(nullptr);
    }

    trapezoids.erase(trapezoid);

    if (trapezoid->getLowerLeftTrapezoid() != nullptr){
        if (trapezoid->getLowerLeftTrapezoid()->getLowerRightTrapezoid() == trapezoid){
            trapezoid->getLowerLeftTrapezoid()->setLowerRightTrapezoid(nullptr);
        }

        if (trapezoid->getLowerLeftTrapezoid()->getUpperRightTrapezoid() == trapezoid){
            trapezoid->getLowerLeftTrapezoid()->setUpperRightTrapezoid(nullptr);
        }
    }

    if (trapezoid->getUpperLeftTrapezoid() != nullptr){
        if (trapezoid->getUpperLeftTrapezoid()->getLowerRightTrapezoid() == trapezoid){
            trapezoid->getUpperLeftTrapezoid()->setLowerRightTrapezoid(nullptr);
        }

        if (trapezoid->getUpperLeftTrapezoid()->getUpperRightTrapezoid() == trapezoid){
            trapezoid->getUpperLeftTrapezoid()->setUpperRightTrapezoid(nullptr);
        }
    }

    if (trapezoid->getLowerRightTrapezoid() != nullptr){
        if (trapezoid->getLowerRightTrapezoid()->getLowerLeftTrapezoid() == trapezoid){
            trapezoid->getLowerRightTrapezoid()->setLowerLeftTrapezoid(nullptr);
        }

        if (trapezoid->getLowerRightTrapezoid()->getUpperLeftTrapezoid() == trapezoid){
            trapezoid->getLowerRightTrapezoid()->setUpperLeftTrapezoid(nullptr);
        }
    }

    if (trapezoid->getUpperRightTrapezoid() != nullptr){
        if (trapezoid->getUpperRightTrapezoid()->getLowerLeftTrapezoid() == trapezoid){
            trapezoid->getUpperRightTrapezoid()->setLowerLeftTrapezoid(nullptr);
        }

        if (trapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid() == trapezoid){
            trapezoid->getUpperRightTrapezoid()->setUpperLeftTrapezoid(nullptr);
        }
    }

    delete trapezoid;
    trapezoid = nullptr;
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

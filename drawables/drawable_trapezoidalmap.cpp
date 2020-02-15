#include "drawable_trapezoidalmap.h"


DrawableTrapezoidalMap::DrawableTrapezoidalMap(const int &boundingbox, TrapezoidalMapDataset &trapezoidalMapDataset) :
    TrapezoidalMap(boundingbox, trapezoidalMapDataset)
{
}

void DrawableTrapezoidalMap::draw() const
{
    //assert(findIntersectionPoint(cg3::Segment2d(cg3::Point2d(2, 5), cg3::Point2d(8, 3)), cg3::Point2d(1, 2)).y() == -8);

    if (getTrapezoids().size() == 1)
        return;

    for (const auto& trapezoid: getTrapezoids()) {
        glColor3ub(trapezoid->getColor().red(), trapezoid->getColor().green(), trapezoid->getColor().blue());

        const cg3::Segment2d top = trapezoidalMapDataset.getSegment(trapezoid->getTop());
        const cg3::Segment2d bottom = trapezoidalMapDataset.getSegment(trapezoid->getBottom());

        const cg3::Point2d leftp = trapezoidalMapDataset.getPoint(trapezoid->getLeftp());
        const cg3::Point2d rightp = trapezoidalMapDataset.getPoint(trapezoid->getRightp());

        cg3::Point2d bottomLeftIntersection = tmap::findIntersectionPoint(bottom, leftp);
        cg3::Point2d bottomRightIntersection = tmap::findIntersectionPoint(bottom, rightp);
        cg3::Point2d topRightIntersection = tmap::findIntersectionPoint(top, rightp);
        cg3::Point2d topLeftIntersection = tmap::findIntersectionPoint(top, leftp);

        glBegin(GL_POLYGON);
          glVertex2d(bottomLeftIntersection.x(), bottomLeftIntersection.y());
          glVertex2d(bottomRightIntersection.x(), bottomRightIntersection.y());
          glVertex2d(topRightIntersection.x(), topRightIntersection.y());
          glVertex2d(topLeftIntersection.x(), topLeftIntersection.y());
        glEnd();

        #if YESDEBUG
            // Debug point of trapezoids
            glBegin(GL_POINTS);
                glColor3ub(255, 0, 0); glVertex2d(bottomLeftIntersection.x(), bottomLeftIntersection.y());      //red
                glColor3ub(225, 255, 0); glVertex2d(bottomRightIntersection.x(), bottomRightIntersection.y());  //yellow
                glColor3ub(0, 247, 255); glVertex2d(topRightIntersection.x(), topRightIntersection.y());        //blue
                glColor3ub(217, 0, 255); glVertex2d(topLeftIntersection.x(), topLeftIntersection.y());          //purple
            glEnd();
        #endif
    }
}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = this->getBoundingBox();
    return boundingBox.diag();
}

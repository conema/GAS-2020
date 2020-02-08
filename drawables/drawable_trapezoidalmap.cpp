#include "drawable_trapezoidalmap.h"

DrawableTrapezoidalMap::DrawableTrapezoidalMap()
{

}

DrawableTrapezoidalMap::DrawableTrapezoidalMap(const int &boundingbox) :
    TrapezoidalMap(boundingbox)
{
    srand (time(NULL));
}

void DrawableTrapezoidalMap::draw() const
{
    //assert(findIntersectionPoint(cg3::Segment2d(cg3::Point2d(2, 5), cg3::Point2d(8, 3)), cg3::Point2d(1, 2)).y() == -8);

    if (getTrapezoids().size() == 1)
        return;
    int i = 0;
    for (const tg::Trapezoid &trapezoid: getTrapezoids()) {
        double randN = rand()%255;

        glColor3ub(randN, randN, randN);


        cg3::Point2d bottomLeftIntersection = findIntersectionPoint(trapezoid.getBottom(), trapezoid.getLeftp());
        cg3::Point2d bottomRightIntersection = findIntersectionPoint(trapezoid.getBottom(), trapezoid.getRightp());
        cg3::Point2d topRightIntersection = findIntersectionPoint(trapezoid.getTop(), trapezoid.getRightp());
        cg3::Point2d topLeftIntersection = findIntersectionPoint(trapezoid.getTop(), trapezoid.getLeftp());

        glBegin(GL_POLYGON);
          glVertex2d(bottomLeftIntersection.x(), bottomLeftIntersection.y());
          glVertex2d(bottomRightIntersection.x(), bottomRightIntersection.y());
          glVertex2d(topRightIntersection.x(), topRightIntersection.y());
          glVertex2d(topLeftIntersection.x(), topLeftIntersection.y());
        glEnd();

        if ( i == 2) {

        glBegin(GL_POINTS);
            glColor3ub(255, 0, 0); glVertex2d(bottomLeftIntersection.x(), bottomLeftIntersection.y());      //red
            glColor3ub(225, 255, 0); glVertex2d(bottomRightIntersection.x(), bottomRightIntersection.y());  //yellow
            glColor3ub(0, 247, 255); glVertex2d(topRightIntersection.x(), topRightIntersection.y());        //blue
            glColor3ub(217, 0, 255); glVertex2d(topLeftIntersection.x(), topLeftIntersection.y());          //purple
        glEnd();
        }

        i++;
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

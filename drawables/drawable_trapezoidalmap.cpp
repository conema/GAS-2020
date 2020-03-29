#include "drawable_trapezoidalmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

DrawableTrapezoidalMap::DrawableTrapezoidalMap(const int &boundingbox) :
    TrapezoidalMap(boundingbox),
    highlightedTrapezoid(nullptr)
{
}

void DrawableTrapezoidalMap::draw() const
{
    //assert(findIntersectionPoint(cg3::Segment2d(cg3::Point2d(2, 5), cg3::Point2d(8, 3)), cg3::Point2d(1, 2)).y() == -8);

    if (getTrapezoids().size() == 1)
        return;

    for (const auto& trapezoid: getTrapezoids()) {
        if (trapezoid != nullptr && trapezoid == getHighlightedTrapezoid()){
            glColor3ub(0, 0, 255);
        } else {
            glColor3ub(trapezoid->getColor().red(), trapezoid->getColor().green(), trapezoid->getColor().blue());
        }


        /* Controlli di debug */
        if ((trapezoid->getUpperLeftTrapezoid() ==  nullptr && trapezoid->getLowerLeftTrapezoid() != nullptr) ||
                (trapezoid->getUpperLeftTrapezoid() !=  nullptr && trapezoid->getLowerLeftTrapezoid() == nullptr) ||
                (trapezoid->getUpperRightTrapezoid() !=  nullptr && trapezoid->getLowerRightTrapezoid() == nullptr) ||
                (trapezoid->getUpperRightTrapezoid() ==  nullptr && trapezoid->getLowerRightTrapezoid() != nullptr)){
            std::cerr<<trapezoid<<std::endl;
            if (trapezoid->getUpperLeftTrapezoid() == nullptr){
                std::cerr<<"Upper Left is null"<<std::endl;
            }

            if (trapezoid->getLowerLeftTrapezoid() == nullptr){
                std::cerr<<"Lower Left is null"<<std::endl;
            }

            if (trapezoid->getUpperRightTrapezoid() == nullptr){
                std::cerr<<"Upper right is null"<<std::endl;
            }

            if (trapezoid->getLowerRightTrapezoid() == nullptr){
                std::cerr<<"Lower right is null"<<std::endl;
            }
        }

        if (trapezoid->getLeaf() == nullptr){
            std::cerr<<trapezoid<<" "<<"nullptr node"<<std::endl;
        }

        if ((trapezoid->getUpperLeftTrapezoid() != nullptr && trapezoid->getUpperLeftTrapezoid()->getUpperRightTrapezoid() != trapezoid) && (trapezoid->getUpperLeftTrapezoid() != nullptr && trapezoid->getUpperLeftTrapezoid()->getLowerRightTrapezoid() != trapezoid)){
            std::cerr<<trapezoid<<" "<<"problema con upper left"<<std::endl;
        }

        if ((trapezoid->getLowerLeftTrapezoid() != nullptr && trapezoid->getLowerLeftTrapezoid()->getUpperRightTrapezoid() != trapezoid) && (trapezoid->getLowerLeftTrapezoid() != nullptr && trapezoid->getLowerLeftTrapezoid()->getLowerRightTrapezoid() != trapezoid)){
            std::cerr<<trapezoid<<" "<<"problema con lower left"<<std::endl;
        }

        if ((trapezoid->getUpperRightTrapezoid() != nullptr && trapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid() != trapezoid) && (trapezoid->getUpperRightTrapezoid() != nullptr && trapezoid->getUpperRightTrapezoid()->getLowerLeftTrapezoid() != trapezoid)){
            std::cerr<<trapezoid<<" "<<"problema con upper Right"<<std::endl;
        }

        if ((trapezoid->getLowerRightTrapezoid() != nullptr && trapezoid->getLowerRightTrapezoid()->getUpperLeftTrapezoid() != trapezoid) && (trapezoid->getLowerRightTrapezoid()  != nullptr && trapezoid->getLowerRightTrapezoid()->getLowerLeftTrapezoid() != trapezoid)){
            std::cerr<<trapezoid<<" "<<"problema con lower right"<<std::endl;
        }

        /* Fine controlli di debug */

        const cg3::Segment2d top = getSegment(trapezoid->getTop());
        const cg3::Segment2d bottom = getSegment(trapezoid->getBottom());

        const cg3::Point2d leftp = getPoint(trapezoid->getLeftp());
        const cg3::Point2d rightp = getPoint(trapezoid->getRightp());

        cg3::Point2d bottomLeftIntersection = tmap::findIntersectionPoint(bottom, leftp);
        cg3::Point2d bottomRightIntersection = tmap::findIntersectionPoint(bottom, rightp);
        cg3::Point2d topRightIntersection = tmap::findIntersectionPoint(top, rightp);
        cg3::Point2d topLeftIntersection = tmap::findIntersectionPoint(top, leftp);


        // Draw trapezoid area
        glBegin(GL_POLYGON);
          glVertex2d(bottomLeftIntersection.x(), bottomLeftIntersection.y());
          glVertex2d(bottomRightIntersection.x(), bottomRightIntersection.y());
          glVertex2d(topRightIntersection.x(), topRightIntersection.y());
          glVertex2d(topLeftIntersection.x(), topLeftIntersection.y());
        glEnd();

        // Draw trapezoid boundary
        cg3::opengl::drawLine2(topLeftIntersection, bottomLeftIntersection, cg3::Color(255, 0, 0), 3);
        cg3::opengl::drawLine2(topRightIntersection, bottomRightIntersection, cg3::Color(255, 0, 0), 3);

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

tmap::Trapezoid *DrawableTrapezoidalMap::getHighlightedTrapezoid() const
{
    return highlightedTrapezoid;
}

void DrawableTrapezoidalMap::setHighlightedTrapezoid(tmap::Trapezoid *value)
{
    highlightedTrapezoid = value;
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

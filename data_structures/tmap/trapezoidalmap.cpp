#include "trapezoidalmap.h"


tmap::TrapezoidalMap::TrapezoidalMap(const int &boundingbox):
    boundingBox(cg3::Point2d(-boundingbox, -boundingbox), cg3::Point2d(boundingbox,boundingbox))
{


}

// Rule of five
tmap::TrapezoidalMap::TrapezoidalMap(tmap::TrapezoidalMap &&in)
{
    this->trapezoids = in.trapezoids;
    in.trapezoids.clear();
}

tmap::TrapezoidalMap &tmap::TrapezoidalMap::operator =(tmap::TrapezoidalMap &&in)
{
    std::swap(trapezoids, in.trapezoids);
    return *this;
}

/**
 * @brief Duplicate a trapezoidal map by using a map with the old pointer of the trapezoid and the new pointer
 */
tmap::TrapezoidalMap::TrapezoidalMap(const tmap::TrapezoidalMap &of)
{
    std::unordered_map<Trapezoid*, Trapezoid*> newTrapezoids;
    Trapezoid* newTrapezoid = nullptr;
    std::vector<Trapezoid*> neighbors;
    Trapezoid* neighbor = nullptr;
    Trapezoid* newNeighbor = nullptr;

    for (Trapezoid* trapezoid: of.getTrapezoids()) {

        // If the trapezoid is present in the newTrapezoids map, we don't need to recreate it
        if (newTrapezoids.find(trapezoid) == newTrapezoids.end()){
            newTrapezoid = new tmap::Trapezoid(*trapezoid);
            addTrapezoid(newTrapezoid);
            newTrapezoids.insert(std::make_pair(trapezoid, newTrapezoid));
        } else {
            newTrapezoid = newTrapezoids.find(trapezoid)->second;
        }

        neighbors.push_back(trapezoid->getLowerLeftTrapezoid());
        neighbors.push_back(trapezoid->getUpperLeftTrapezoid());
        neighbors.push_back(trapezoid->getUpperRightTrapezoid());
        neighbors.push_back(trapezoid->getLowerRightTrapezoid());

        // Neighbors
        for(int i = 0; i <= 3; i++){
            neighbor = neighbors.at(i);

            std::unordered_map<Trapezoid*, Trapezoid*>::iterator searchTrapezoid = newTrapezoids.find(neighbor);
            if (searchTrapezoid == newTrapezoids.end()){
                newNeighbor = new tmap::Trapezoid(*neighbor);
                addTrapezoid(newNeighbor);
                newTrapezoids.insert(std::make_pair(trapezoid, newNeighbor));

                if (i == 0){
                    newTrapezoid->setLowerLeftTrapezoid(newNeighbor);
                } else if(i == 1){
                    newTrapezoid->setUpperLeftTrapezoid(newNeighbor);
                } else if(i == 2){
                    newTrapezoid->setUpperRightTrapezoid(newNeighbor);
                } else if (i == 3) {
                    newTrapezoid->setLowerRightTrapezoid(newNeighbor);
                }
            } else {
                // The neighbors has already been re-created
                if (i == 0){
                    newTrapezoid->setLowerLeftTrapezoid(searchTrapezoid->second);
                } else if(i == 1){
                    newTrapezoid->setUpperLeftTrapezoid(searchTrapezoid->second);
                } else if(i == 2){
                    newTrapezoid->setUpperRightTrapezoid(searchTrapezoid->second);
                } else if (i == 3) {
                    newTrapezoid->setLowerRightTrapezoid(searchTrapezoid->second);
                }
            }
        }
    }
}

tmap::TrapezoidalMap &tmap::TrapezoidalMap::operator=(tmap::TrapezoidalMap other)
{
    swap(other);
    return*this;
}

tmap::TrapezoidalMap::~TrapezoidalMap()
{
    removeTrapezoids();
}

void tmap::TrapezoidalMap::swap(TrapezoidalMap& other){
    using std::swap;
    swap(trapezoids, other.trapezoids);
}

/**
 * @brief Initialize the trapezoid map by creating two fake segment for the top and the bottom and a trapezoid for the bounding box
 */
void tmap::TrapezoidalMap::initializeTrapezoids()
{
    const int &boundingbox = getBoundingBox().lengthX()/2;

    cg3::Point2d leftp = cg3::Point2d(-boundingbox, -boundingbox);
    cg3::Point2d rightp = cg3::Point2d(boundingbox, boundingbox);

    // Initialize Trapezoidal Map with the boundingbox trapezoid (S0)
    cg3::Segment2d top = cg3::Segment2d(cg3::Point2d(-boundingbox, boundingbox), rightp);
    cg3::Segment2d bottom = cg3::Segment2d(leftp, cg3::Point2d(boundingbox, -boundingbox));

    bool success;

    addSegment(top, success);
    assert(success);

    addSegment(bottom, success);
    assert(success);

    size_t id_top = findSegment(top, success);
    assert(success);

    size_t id_bottom = findSegment(bottom, success);
    assert(success);

    size_t id_leftp = findPoint(leftp, success);
    assert(success);

    size_t id_rightp = findPoint(rightp, success);
    assert(success);

    tmap::Trapezoid *S0 = new tmap::Trapezoid(id_top,
                     id_bottom,
                     id_rightp,
                     id_leftp);

    S0->Trapezoid::updateAdjacencies(nullptr, nullptr, nullptr, nullptr);


    addTrapezoid(S0);
}

/**
 * @brief add a new trapezoid to the map
 * @param trapezoid
 */
void tmap::TrapezoidalMap::addTrapezoid(tmap::Trapezoid *trapezoid)
{
    trapezoids.insert(trapezoid);
}

/**
 * @brief remove a trapezoid from the map and set its neighbors to him pointers to nullptr
 * @param[in] trapezoid
 */
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
}

/**
 * @brief get the boundingbox of the map
 * @return the bounding box
 */
const cg3::BoundingBox2& tmap::TrapezoidalMap::getBoundingBox() const
{
    return boundingBox;
}

const tmap::TrapezoidalMap::Trapezoids &tmap::TrapezoidalMap::getTrapezoids() const
{
    return trapezoids;
}

/**
 * @brief Remove all the trapezoids of the map, segments and points included
 */
void tmap::TrapezoidalMap::removeTrapezoids()
{
    for (const auto& trapezoid: trapezoids) {
        delete trapezoid;
    }

    trapezoids.clear();

    points.clear();
    indexedSegments.clear();
    pointMap.clear();
    segmentMap.clear();
}

/**
 * @brief add a new point to the pointMap and assign the id to the point
 * @param[in] point
 * @param pointInserted
 * @return the point id
 */
size_t tmap::TrapezoidalMap::addPoint(const cg3::Point2d& point, bool& pointInserted)
{
    bool found;
    size_t id = findPoint(point, found);

    pointInserted = false;
    if (!found){
        pointInserted = true;
        id = points.size();

        //Add point
        points.push_back(point);
        pointMap.insert(std::make_pair(point, id));
    }

    return id;
}

/**
 * @brief add a new segment to the segmentMap and assign the id to the segment
 * @param segment
 * @param segmentInserted
 * @return the id of the segment
 */
size_t tmap::TrapezoidalMap::addSegment(const cg3::Segment2d& segment, bool& segmentInserted)
{
    size_t id;
    bool found;

    cg3::Segment2d orderedSegment = segment;
    if (segment.p2() < segment.p1()) {
        orderedSegment.setP1(segment.p2());
        orderedSegment.setP2(segment.p1());
    }

    findSegment(orderedSegment, found);

    id = std::numeric_limits<size_t>::max();

    segmentInserted = false;
    if (!found) {
        bool foundPoint1;
        size_t id1 = findPoint(orderedSegment.p1(), foundPoint1);
        bool foundPoint2;
        size_t id2 = findPoint(orderedSegment.p2(), foundPoint2);

        segmentInserted = true;

        id = indexedSegments.size();

        if (!foundPoint1) {
            bool insertedPoint1;
            id1 = addPoint(orderedSegment.p1(), insertedPoint1);
            assert(insertedPoint1);
        }

        if (!foundPoint2) {
            bool insertedPoint2;
            id2 = addPoint(orderedSegment.p2(), insertedPoint2);
            assert(insertedPoint2);
        }

        IndexedSegment2d indexedSegment(id1, id2);
        if (indexedSegment.second < indexedSegment.first) {
            std::swap(indexedSegment.first, indexedSegment.second);
        }

        indexedSegments.push_back(indexedSegment);

        segmentMap.insert(std::make_pair(indexedSegment, id));
    }

    return id;
}

/**
 * @brief find the point in the map and return its id
 * @param point
 * @param found
 * @return the point id
 */
size_t tmap::TrapezoidalMap::findPoint(const cg3::Point2d &point, bool &found)
{
    std::unordered_map<cg3::Point2d, size_t>::iterator it = pointMap.find(point);

    //Point already in the data structure
    if (it != pointMap.end()) {
        found = true;
        return it->second;
    }
    //Point not in the data structure
    else {
        found = false;
        return std::numeric_limits<size_t>::max();
    }
}

/**
 * @brief find the segment in the map and return its id
 * @param segment
 * @param found
 * @return the segment id
 */
size_t tmap::TrapezoidalMap::findSegment(const cg3::Segment2d& segment, bool& found)
{
    found = false;

    cg3::Segment2d orderedSegment = segment;
    if (segment.p2() < segment.p1()) {
        orderedSegment.setP1(segment.p2());
        orderedSegment.setP2(segment.p1());
    }

    bool foundPoint1;
    size_t id1 = findPoint(orderedSegment.p1(), foundPoint1);
    if (!foundPoint1)
        return std::numeric_limits<size_t>::max();

    bool foundPoint2;
    size_t id2 = findPoint(orderedSegment.p2(), foundPoint2);
    if (!foundPoint2)
        return std::numeric_limits<size_t>::max();

    return findIndexedSegment(IndexedSegment2d(id1, id2), found);
}

/**
 * @brief find an indexed segment and return the id
 * @param indexedSegment
 * @param found
 * @return the id of the indexed segment
 */
size_t tmap::TrapezoidalMap::findIndexedSegment(const IndexedSegment2d& indexedSegment, bool& found)
{
    IndexedSegment2d orderedIndexedSegment = indexedSegment;
    if (indexedSegment.second < indexedSegment.first) {
        orderedIndexedSegment.first = indexedSegment.second;
        orderedIndexedSegment.second = indexedSegment.first;
    }

    std::unordered_map<IndexedSegment2d, size_t>::iterator it = segmentMap.find(orderedIndexedSegment);

    //Segment already in the data structure
    if (it != segmentMap.end()) {
        found = true;
        return it->second;
    }
    //Segment not in the data structure
    else {
        found = false;
        return std::numeric_limits<size_t>::max();
    }
}

/**
 * @brief get the point by the id
 * @param id
 * @return the point
 */
const cg3::Point2d& tmap::TrapezoidalMap::getPoint(size_t id) const
{
    return points[id];
}

/**
 * @brief get the segment by the id
 * @param id
 * @return the point
 */
cg3::Segment2d tmap::TrapezoidalMap::getSegment(size_t id) const
{
    return cg3::Segment2d(points[indexedSegments[id].first], points[indexedSegments[id].second]);
}


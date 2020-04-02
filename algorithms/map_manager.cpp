#include "map_manager.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param segment
 * @param trapezoidalMap
 * @param dag
 */
void tbuild::buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag)
{
    if (trapezoidalMap.getTrapezoids().size() == 0){
        // Initialize trapezoidalmap and dag
        trapezoidalMap.initializeTrapezoids();
        dag.initializeDag(new dag::Leaf(*trapezoidalMap.getTrapezoids().begin()));
    }

    // Add segments to map
    bool segmentFound = false;
    trapezoidalMap.addSegment(segment, segmentFound);

    // Find trapezoids intersected by the new segment
    std::vector<tmap::Trapezoid*> tl = tbuild::followSegment(dag, segment, trapezoidalMap);

    cg3::Point2d leftMost = segment.p1();
    cg3::Point2d rightMost = segment.p2();

    // Re-order the segment points, p1 leftmost and p2 rightmost
    tmap::orderPoint(leftMost, rightMost);

    bool success;

    const size_t leftMostId = trapezoidalMap.findPoint(leftMost, success);
    const size_t rightMostId = trapezoidalMap.findPoint(rightMost, success);
    const size_t segmentId = trapezoidalMap.findSegment(segment, success);

    if (tl.size() == 1) {
        // 1 trapezoid intersected

        tbuild::oneTrapezoidIntersected(trapezoidalMap, dag, segmentId, leftMostId, rightMostId, *tl.begin());
    } else if (tl.size() > 1) {
        // >1 Trapezoid intersected

        const tmap::Trapezoid* trapezoidFirstEndpoint = *tl.begin();
        const tmap::Trapezoid* trapezoidSecondEndpoint = tl.back();

        /*
         * oldUpper and oldLower are used to memorize the upper and lower trapezoids of the precedent build, for example,
         * after the execution of multiTrapezoidIntersectedFirstEndpoint, I'll update these pointers with the correct
         * upper and lower. After, in the execution of multiTrapezoidIntersectedMiddle or of multiTrapezoidIntersectedLastEndpoint,
         * I'll check if the new trapezoids that I'm creating can be merged with the precedent upper or lower trapezoids.
         */
        tmap::Trapezoid* oldUpper = nullptr;
        tmap::Trapezoid* oldLower = nullptr;

        // First endpoint
        tbuild::multiTrapezoidIntersectedFirstEndpoint(trapezoidalMap, dag, segmentId, leftMostId, trapezoidFirstEndpoint, oldUpper, oldLower);

        // Middle trapezoids
        for (auto it = tl.begin()+1; it != tl.end()-1; ++it){
            tbuild::multiTrapezoidIntersectedMiddle(trapezoidalMap, dag, segmentId, *it, oldUpper, oldLower);
        }

        // Right endpoint
        tbuild::multiTrapezoidIntersectedLastEndpoint(trapezoidalMap, dag, segmentId, rightMostId, trapezoidSecondEndpoint, oldUpper, oldLower);

        // Remove first and last trapezoids
        trapezoidalMap.removeTrapezoid(*tl.begin());
        trapezoidalMap.removeTrapezoid(tl.back());
    }
}

/**
 * @brief Build the Trapezoidal Map and DAG in the case where there is only one trapezoid intersected
 * @param trapezoidalMap
 * @param dag
 * @param segmentId
 * @param leftMostId
 * @param rightMostId
 * @param trapezoid
 */
void tbuild::oneTrapezoidIntersected(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &leftMostId, const size_t &rightMostId, tmap::Trapezoid * const &trapezoid){
    /* -----  TrapezoidalMap building  -----  */
    // 4 new trapezoids for the segment
    tmap::Trapezoid *A = new tmap::Trapezoid(trapezoid->getTop(),
                                             trapezoid->getBottom(),
                                             leftMostId,
                                             trapezoid->getLeftp());

    tmap::Trapezoid *B = new tmap::Trapezoid(trapezoid->getTop(),
                                             trapezoid->getBottom(),
                                             trapezoid->getRightp(),
                                             rightMostId);

    tmap::Trapezoid *C = new tmap::Trapezoid(trapezoid->getTop(),
                                             segmentId,
                                             rightMostId,
                                             leftMostId);

    tmap::Trapezoid *D = new tmap::Trapezoid(segmentId,
                                             trapezoid->getBottom(),
                                             rightMostId,
                                             leftMostId);

    // Add adjacencies
    A->Trapezoid::updateAdjacencies(trapezoid->getLowerLeftTrapezoid(),
                                    trapezoid->getUpperLeftTrapezoid(),
                                    C,
                                    D);



    B->Trapezoid::updateAdjacencies(D,
                                    C,
                                    trapezoid->getUpperRightTrapezoid(),
                                    trapezoid->getLowerRightTrapezoid());



    C->Trapezoid::updateAdjacencies(A,
                                    A,
                                    B,
                                    B);

    D->Trapezoid::updateAdjacencies(A,
                                    A,
                                    B,
                                    B);


    // Update left neighbors of trapezoid
    if (trapezoid->getLowerLeftTrapezoid() != nullptr){
        // Case where the new segment is in the LOWER right neighbor

        if (trapezoid == trapezoid->getLowerLeftTrapezoid()->getLowerRightTrapezoid()){
            trapezoid->getLowerLeftTrapezoid()->setLowerRightTrapezoid(A);
        }

        if (trapezoid == trapezoid->getLowerLeftTrapezoid()->getUpperRightTrapezoid()) {
            trapezoid->getLowerLeftTrapezoid()->setUpperRightTrapezoid(A);
        }

    }

    if (trapezoid->getUpperLeftTrapezoid() != nullptr){
        // Case where the new segment is in the UPPER right neighbor

        if (trapezoid == trapezoid->getUpperLeftTrapezoid()->getUpperRightTrapezoid()){
            trapezoid->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A);
        }

        if(trapezoid == trapezoid->getUpperLeftTrapezoid()->getLowerRightTrapezoid()) {
            trapezoid->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A);
        }

    }

    // Update right neighbors of trapezoid
    if (trapezoid->getLowerRightTrapezoid() != nullptr){
        // Case where the new segment is in the LOWER left neighbor
        if (trapezoid == trapezoid->getLowerRightTrapezoid()->getLowerLeftTrapezoid()){
            if (trapezoid == trapezoid->getLowerRightTrapezoid()->getLowerLeftTrapezoid()){
                trapezoid->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B);
            }

            if (trapezoid == trapezoid->getLowerRightTrapezoid()->getUpperLeftTrapezoid()){
                trapezoid->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B);
            }

        }
    }

    if (trapezoid->getUpperRightTrapezoid() != nullptr){
        // Case where the new segment is in the UPPER left neighbor
        if (trapezoid == trapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid()){

            if (trapezoid == trapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid()){
                trapezoid->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B);
            }

            if (trapezoid == trapezoid->getUpperRightTrapezoid()->getLowerLeftTrapezoid()){
                trapezoid->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B);
            }
        }

    }


    trapezoidalMap.addTrapezoid(A);
    trapezoidalMap.addTrapezoid(B);
    trapezoidalMap.addTrapezoid(C);
    trapezoidalMap.addTrapezoid(D);

    /* ----- DAG nodes building -----  */
    // Create x-nodes and y-nodes
    dag::XNode *pi = new dag::XNode(leftMostId);
    dag::XNode *qi = new dag::XNode(rightMostId);
    dag::YNode *si = new dag::YNode(segmentId);

    // Create leaves with trapezoids

    dag::Leaf *Al = nullptr;
    Al = new dag::Leaf(A);
    Al->getFathers().insert(pi);

    dag::Leaf *Bl = nullptr;
    Bl = new dag::Leaf(B);

    Bl->getFathers().insert(qi);

    dag::Leaf *Cl = new dag::Leaf(C);
    Cl->getFathers().insert(si);

    dag::Leaf *Dl = new dag::Leaf(D);
    Dl->getFathers().insert(si);

    // Find node on the DAG
    dag::Node *foundNode = trapezoid->getLeaf();

    if (foundNode->getFathers().size() == 0){
        // The foundNode is the root and it's substituted with si
        dag.setRoot(pi);
    }else {
        // Update the children of the father of foundNode
        dag.updateChildren(foundNode, pi);
    }

    pi->setLeftChild(Al);
    pi->setRightChild(qi);
    pi->getFathers() = foundNode->getFathers();

    qi->setLeftChild(si);
    qi->setRightChild(Bl);

    qi->getFathers().insert(pi);

    si->setLeftChild(Cl);
    si->setRightChild(Dl);
    si->getFathers().insert(qi);

    // Link between trapezoids and leaves
    A->setLeaf(Al);
    B->setLeaf(Bl);
    C->setLeaf(Cl);
    D->setLeaf(Dl);

    trapezoidalMap.removeTrapezoid(trapezoid);
    dag.deleteNode(foundNode);
    foundNode = nullptr;
}

/**
 * @brief Build the Trapezoidal Map and DAG in the case where there are >1 trapezoid intersected, this is for building 3 new trapezoid on the first intersected trapezoid
 * @param[in] trapezoidalMap
 * @param[in] dag
 * @param[in] segmentId
 * @param[in] leftMostId
 * @param[in] trapezoidFirstEndpoint: the trapezoid intersected with the first point of the segment
 * @param[out] oldUpper: the old upper trapezoid (B), needed to merge the upper trapezoids
 * @param[out] oldLower: the old lower trapezoid (C), needed to merge the lower trapezoids
 */
void tbuild::multiTrapezoidIntersectedFirstEndpoint(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &leftMostId, const tmap::Trapezoid* const &trapezoidFirstEndpoint, tmap::Trapezoid* &oldUpper, tmap::Trapezoid* &oldLower) {
    /* -----  Left endpoint  -----  */
    // 3 new trapezoids for the left endpoint of the segment
    // Left trapezoid
    tmap::Trapezoid *A = new tmap::Trapezoid(trapezoidFirstEndpoint->getTop(),
                                             trapezoidFirstEndpoint->getBottom(),
                                             leftMostId,
                                             trapezoidFirstEndpoint->getLeftp());

    // Top trapezoid
    tmap::Trapezoid *B = new tmap::Trapezoid(trapezoidFirstEndpoint->getTop(),
                                             segmentId,
                                             trapezoidFirstEndpoint->getRightp(),
                                             leftMostId);

    // Bottom trapezoid
    tmap::Trapezoid *C = new tmap::Trapezoid(segmentId,
                                             trapezoidFirstEndpoint->getBottom(),
                                             trapezoidFirstEndpoint->getRightp(),
                                             leftMostId );

    // Add adjacencies
    A->Trapezoid::updateAdjacencies(trapezoidFirstEndpoint->getLowerLeftTrapezoid(),
                                    trapezoidFirstEndpoint->getUpperLeftTrapezoid(),
                                    B,
                                    C);

    B->Trapezoid::updateAdjacencies(A,
                                    A,
                                    trapezoidFirstEndpoint->getUpperRightTrapezoid(),
                                    nullptr);

    C->Trapezoid::updateAdjacencies(A,
                                    A,
                                    nullptr,
                                    trapezoidFirstEndpoint->getLowerRightTrapezoid());

    // Update neighbors' neighbors, they are updated only if the pointers are nullptr, otherwise they are valid pointers to some trapezoids
    if (trapezoidFirstEndpoint->getLowerLeftTrapezoid() != nullptr){
        if (trapezoidFirstEndpoint->getLowerLeftTrapezoid()->getLowerRightTrapezoid() == trapezoidFirstEndpoint) {
            trapezoidFirstEndpoint->getLowerLeftTrapezoid()->setLowerRightTrapezoid(A);
        }

        if (trapezoidFirstEndpoint->getLowerLeftTrapezoid()->getUpperRightTrapezoid()  == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getLowerLeftTrapezoid()->setUpperRightTrapezoid(A);
        }
    }

    if (trapezoidFirstEndpoint->getUpperLeftTrapezoid() != nullptr){
        if (trapezoidFirstEndpoint->getUpperLeftTrapezoid()->getLowerRightTrapezoid() == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A);
        }

        if (trapezoidFirstEndpoint->getUpperLeftTrapezoid()->getUpperRightTrapezoid() == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A);
        }
    }

    if (trapezoidFirstEndpoint->getLowerRightTrapezoid() != nullptr){
        if (trapezoidFirstEndpoint->getLowerRightTrapezoid()->getLowerLeftTrapezoid() == trapezoidFirstEndpoint) {
            trapezoidFirstEndpoint->getLowerRightTrapezoid()->setLowerLeftTrapezoid(C);
        }

        if (trapezoidFirstEndpoint->getLowerRightTrapezoid()->getUpperLeftTrapezoid() == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getLowerRightTrapezoid()->setUpperLeftTrapezoid(C);
        }
    }

    if (trapezoidFirstEndpoint->getUpperRightTrapezoid() != nullptr){
        if (trapezoidFirstEndpoint->getUpperRightTrapezoid()->getLowerLeftTrapezoid() == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B);
        }

        if (trapezoidFirstEndpoint->getUpperRightTrapezoid()->getUpperLeftTrapezoid() == trapezoidFirstEndpoint){
            trapezoidFirstEndpoint->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B);
        }
    }

    trapezoidalMap.addTrapezoid(A);
    trapezoidalMap.addTrapezoid(B);
    trapezoidalMap.addTrapezoid(C);

    // DAG nodes building for left endpoint
    dag::XNode *xNode1 = new dag::XNode(leftMostId);
    dag::YNode *yNode1 = new dag::YNode(segmentId);
    dag::Leaf *Al = new dag::Leaf(A);
    dag::Leaf *Bl = new dag::Leaf(B);
    dag::Leaf *Cl = new dag::Leaf(C);

    xNode1->setLeftChild(Al);
    xNode1->setRightChild(yNode1);

    yNode1->setLeftChild(Bl);
    yNode1->setRightChild(Cl);

    A->setLeaf(Al);
    B->setLeaf(Bl);
    C->setLeaf(Cl);

    // Find the node in the DAG and replace it with the new xNode
    dag::Leaf *deleteLeaf = trapezoidFirstEndpoint->getLeaf();

    dag.updateChildren(deleteLeaf, xNode1);


    xNode1->getFathers() = deleteLeaf->getFathers();
    yNode1->getFathers().insert(xNode1);

    Al->getFathers().insert(xNode1);
    Bl->getFathers().insert(yNode1);
    Cl->getFathers().insert(yNode1);

    dag.deleteNode(deleteLeaf);
    deleteLeaf = nullptr;

    oldUpper = B;
    oldLower = C;
}

/**
 * @brief Build the Trapezoidal Map and DAG in the case where there are >1 trapezoid intersected, this is for building 2 new trapezoid on each trapezoids between the two segment endpoints
 * @param trapezoidalMap
 * @param dag
 * @param segmentId
 * @param currentTrapezoid: the current intersected trapezoids
 * @param oldUpper
 * @param oldLower
 */
void tbuild::multiTrapezoidIntersectedMiddle(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, tmap::Trapezoid* const &currentTrapezoid, tmap::Trapezoid* &oldUpper, tmap::Trapezoid* &oldLower) {
    /* -----  Middle of the segment  -----  */
    // Trapezoid in the middle of the segment from tl(1+1) to tl(n-1)
    tmap::Trapezoid *A, *B;


    bool mergeableUpper = (oldUpper->getTop() == currentTrapezoid->getTop() && oldUpper->getBottom() == segmentId);
    bool mergeableLower = (oldLower->getTop() == segmentId && oldLower->getBottom() == currentTrapezoid->getBottom());

    // Merge trapezoid, for every iteration mergableUpper or/and mergeableLower are true
    if (mergeableUpper){
        // Merge upper trapezoid
        oldUpper->setRightp(currentTrapezoid->getRightp());

        A = oldUpper;

        A->setUpperRightTrapezoid(currentTrapezoid->getUpperRightTrapezoid());

        B = new tmap::Trapezoid(segmentId,
                                 currentTrapezoid->getBottom(),
                                 currentTrapezoid->getRightp(),
                                 currentTrapezoid->getLeftp());

        B->Trapezoid::updateAdjacencies(oldLower, oldLower, nullptr, currentTrapezoid->getLowerRightTrapezoid());

        // If the currentTrapezoid has two different right neighbors
        if (currentTrapezoid->getLowerRightTrapezoid() != currentTrapezoid->getUpperRightTrapezoid()){
            if (currentTrapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid() == currentTrapezoid && currentTrapezoid->getUpperRightTrapezoid()->getLowerLeftTrapezoid() == currentTrapezoid) {
                currentTrapezoid->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B);
                currentTrapezoid->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B);

                currentTrapezoid->getUpperRightTrapezoid()->setLowerLeftTrapezoid(A);
                currentTrapezoid->getUpperRightTrapezoid()->setUpperLeftTrapezoid(A);
            }
        }

        // If the currentTrapezoid has two different left neighbors, I should update the new trapezoid with the old left upper neighbor
        if (currentTrapezoid->getLowerLeftTrapezoid() != currentTrapezoid->getUpperLeftTrapezoid()) {
            B->setLowerLeftTrapezoid(currentTrapezoid->getLowerLeftTrapezoid());

            currentTrapezoid->getLowerLeftTrapezoid()->setUpperRightTrapezoid(B);
            currentTrapezoid->getLowerLeftTrapezoid()->setLowerRightTrapezoid(B);
            oldLower->setLowerRightTrapezoid(B);
        }


        oldLower->setUpperRightTrapezoid(B);

        oldLower = B;
        trapezoidalMap.addTrapezoid(B);

    } else if (mergeableLower){
        // Merge lower trapezoid
        oldLower->setRightp(currentTrapezoid->getRightp());

        B = oldLower;

        oldLower->setLowerRightTrapezoid(currentTrapezoid->getLowerRightTrapezoid());

        A = new tmap::Trapezoid(currentTrapezoid->getTop(),
                                 segmentId,
                                 currentTrapezoid->getRightp(),
                                 currentTrapezoid->getLeftp());

        A->Trapezoid::updateAdjacencies(oldUpper, oldUpper, currentTrapezoid->getUpperRightTrapezoid(), nullptr);

        // If the currentTrapezoid has two different left neighbors, I should update the new trapezoid with the old left upper neighbor
        if (currentTrapezoid->getLowerLeftTrapezoid() != currentTrapezoid->getUpperLeftTrapezoid()) {
            A->setUpperLeftTrapezoid(currentTrapezoid->getUpperLeftTrapezoid());

            if (currentTrapezoid->getUpperLeftTrapezoid() != nullptr && currentTrapezoid->getUpperLeftTrapezoid()->getUpperRightTrapezoid() == currentTrapezoid && currentTrapezoid->getUpperLeftTrapezoid()->getLowerRightTrapezoid() == currentTrapezoid) {
                currentTrapezoid->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A);
                currentTrapezoid->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A);
            }
        }

        // If the currentTrapezoid has two different right neighbors
        if (currentTrapezoid->getLowerRightTrapezoid() != currentTrapezoid->getUpperRightTrapezoid()){
            if (currentTrapezoid->getUpperRightTrapezoid()->getUpperLeftTrapezoid() == currentTrapezoid && currentTrapezoid->getUpperRightTrapezoid()->getLowerLeftTrapezoid() == currentTrapezoid) {
                currentTrapezoid->getUpperRightTrapezoid()->setLowerLeftTrapezoid(A);
                currentTrapezoid->getUpperRightTrapezoid()->setUpperLeftTrapezoid(A);

                currentTrapezoid->getLowerRightTrapezoid()->setUpperLeftTrapezoid(oldLower);
                currentTrapezoid->getLowerRightTrapezoid()->setLowerLeftTrapezoid(oldLower);
            }
        }

        // Is this the case where the unmerged top has only one neighbor?
        if (oldUpper->getUpperRightTrapezoid() == currentTrapezoid) {
            oldUpper->setUpperRightTrapezoid(A);
        }

        oldUpper->setLowerRightTrapezoid(A);

        oldUpper = A;

        trapezoidalMap.addTrapezoid(A);
    }

    dag::Leaf *Al;
    dag::Leaf *Bl;

    // Create a new DAG leaf only if the trapezoid is not already inserted
    if (A->getLeaf() == nullptr){
        Al = new dag::Leaf(A);
    } else {
        Al = A->getLeaf();
    }

    if (B->getLeaf() == nullptr){
        Bl = new dag::Leaf(B);
    } else {
        Bl = B->getLeaf();
    }

    dag::YNode *yNode3 = new dag::YNode(segmentId);
    yNode3->setRightChild(Bl);
    yNode3->setLeftChild(Al);

    dag::Node *deleteLeaf2 = currentTrapezoid->getLeaf();

    assert(deleteLeaf2 != nullptr);

    dag.updateChildren(deleteLeaf2, yNode3);

    yNode3->getFathers() = deleteLeaf2->getFathers();
    Al->getFathers().insert(yNode3);
    Bl->getFathers().insert(yNode3);

    A->setLeaf(Al);
    B->setLeaf(Bl);

    dag.deleteNode(deleteLeaf2);
    deleteLeaf2 = nullptr;

    trapezoidalMap.removeTrapezoid(currentTrapezoid);
}

/**
 * @brief Build the Trapezoidal Map and DAG in the case where there are >1 trapezoid intersected, this is for building 3 new trapezoid on the last intersected trapezoid
 * @param trapezoidalMap
 * @param dag
 * @param segmentId
 * @param rightMostId
 * @param trapezoidSecondEndpoint
 * @param oldUpper
 * @param oldLower
 */
void tbuild::multiTrapezoidIntersectedLastEndpoint(tmap::TrapezoidalMap &trapezoidalMap, dag::Dag &dag, const size_t &segmentId, const size_t &rightMostId, const tmap::Trapezoid* const &trapezoidSecondEndpoint, tmap::Trapezoid* &oldUpper, tmap::Trapezoid* &oldLower) {
    /* -----  Right endpoint  -----  */
    // 3 new trapezoids for the right endpoint of the segment
    // Top trapezoid
    tmap::Trapezoid *A2 = new tmap::Trapezoid(trapezoidSecondEndpoint->getTop(),
                                              segmentId,
                                              rightMostId,
                                              trapezoidSecondEndpoint->getLeftp());

    // Right trapezoid
    tmap::Trapezoid *B2 = new tmap::Trapezoid(trapezoidSecondEndpoint->getTop(),
                                              trapezoidSecondEndpoint->getBottom(),
                                              trapezoidSecondEndpoint->getRightp(),
                                              rightMostId);

    // Bottom trapezoid
    tmap::Trapezoid *C2 = new tmap::Trapezoid(segmentId,
                                              trapezoidSecondEndpoint->getBottom(),
                                              rightMostId,
                                              trapezoidSecondEndpoint->getLeftp());

    A2->Trapezoid::updateAdjacencies(oldUpper,
                                     oldUpper,
                                     B2,
                                     B2);

    C2->Trapezoid::updateAdjacencies(oldLower,
                                     oldLower,
                                     B2,
                                     B2);

    bool mergeableUpper = (oldUpper->getTop() == A2->getTop() && oldUpper->getBottom() == segmentId);
    bool mergeableLower = (oldLower->getTop() == C2->getTop() && oldLower->getBottom() == C2->getBottom());

    if (mergeableUpper){
        // Merge upper trapezoid
        oldUpper->setRightp(rightMostId);

        delete A2;
        A2 = oldUpper;

        A2->Trapezoid::updateAdjacencies(A2->getLowerLeftTrapezoid(),
                                         A2->getUpperLeftTrapezoid(),
                                         B2,
                                         B2);

        oldLower->setUpperRightTrapezoid(C2);

        if (trapezoidSecondEndpoint->getUpperLeftTrapezoid() != trapezoidSecondEndpoint->getLowerLeftTrapezoid()){
            oldLower->setLowerRightTrapezoid(C2);
            C2->setLowerLeftTrapezoid(trapezoidSecondEndpoint->getLowerLeftTrapezoid());
        }

        trapezoidalMap.addTrapezoid(C2);
    } else if (mergeableLower){
        // Merge lower trapezoid
        oldLower->setRightp(rightMostId);

        delete C2;
        C2 = oldLower;

        C2->Trapezoid::updateAdjacencies(C2->getLowerLeftTrapezoid(),
                                         C2->getUpperLeftTrapezoid(),
                                         B2,
                                         B2);

        oldUpper->setLowerRightTrapezoid(A2);

        if (trapezoidSecondEndpoint->getUpperLeftTrapezoid() != trapezoidSecondEndpoint->getLowerLeftTrapezoid()){
            A2->setUpperLeftTrapezoid(trapezoidSecondEndpoint->getUpperLeftTrapezoid());

            if(oldUpper->getUpperRightTrapezoid() == trapezoidSecondEndpoint){
                oldUpper->setUpperRightTrapezoid(A2);
            }

            if(oldUpper->getLowerRightTrapezoid() == trapezoidSecondEndpoint){
                oldUpper->setLowerRightTrapezoid(A2);
            }
        }

        // p1 of bottom is >= of p1 of top
        if (tmap::leftMostPoint(trapezoidalMap.getSegment(B2->getBottom())).x() >= tmap::leftMostPoint(trapezoidalMap.getSegment(B2->getTop())).x()){
            // The old trapezoid has A2 has only parent
            oldUpper->setUpperRightTrapezoid(A2);
            A2->setUpperLeftTrapezoid(trapezoidSecondEndpoint->getUpperLeftTrapezoid());
        }

        trapezoidalMap.addTrapezoid(A2);
    }

    B2->Trapezoid::updateAdjacencies(C2,
                                     A2,
                                     trapezoidSecondEndpoint->getUpperRightTrapezoid(),
                                     trapezoidSecondEndpoint->getLowerRightTrapezoid());

    trapezoidalMap.addTrapezoid(B2);


    // Update neighbors' neighbors
    if (trapezoidSecondEndpoint->getLowerRightTrapezoid() != nullptr){
        if (trapezoidSecondEndpoint->getLowerRightTrapezoid()->getLowerLeftTrapezoid() == trapezoidSecondEndpoint){
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B2);
        }

        if (trapezoidSecondEndpoint->getLowerRightTrapezoid()->getUpperLeftTrapezoid() == trapezoidSecondEndpoint){
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }
    }

    if (trapezoidSecondEndpoint->getUpperRightTrapezoid() != nullptr){
        if (trapezoidSecondEndpoint->getUpperRightTrapezoid()->getLowerLeftTrapezoid() == trapezoidSecondEndpoint){
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B2);
        }

        if (trapezoidSecondEndpoint->getUpperRightTrapezoid()->getUpperLeftTrapezoid() == trapezoidSecondEndpoint){
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }
    }

    if (trapezoidSecondEndpoint->getLowerLeftTrapezoid() != nullptr){
        if (trapezoidSecondEndpoint->getLowerLeftTrapezoid()->getLowerRightTrapezoid() == trapezoidSecondEndpoint){
            if (mergeableUpper){
                trapezoidSecondEndpoint->getLowerLeftTrapezoid()->setLowerRightTrapezoid(C2);
            } else if (mergeableLower){
                trapezoidSecondEndpoint->getLowerLeftTrapezoid()->setLowerRightTrapezoid(B2);
            }
        }

        if (trapezoidSecondEndpoint->getLowerLeftTrapezoid()->getUpperRightTrapezoid() == trapezoidSecondEndpoint){
            if (mergeableUpper){
                trapezoidSecondEndpoint->getLowerLeftTrapezoid()->setUpperRightTrapezoid(C2);
            } else if (mergeableLower){
                trapezoidSecondEndpoint->getLowerLeftTrapezoid()->setUpperRightTrapezoid(B2);
            }
        }
    }

    if (trapezoidSecondEndpoint->getUpperLeftTrapezoid() != nullptr){
        if (trapezoidSecondEndpoint->getUpperLeftTrapezoid()->getLowerRightTrapezoid() == trapezoidSecondEndpoint){
            if (mergeableUpper){
                trapezoidSecondEndpoint->getUpperLeftTrapezoid()->setLowerRightTrapezoid(B2);
            } else if (mergeableLower){
                trapezoidSecondEndpoint->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A2);
            }
        }

        if (trapezoidSecondEndpoint->getUpperLeftTrapezoid()->getUpperRightTrapezoid() == trapezoidSecondEndpoint){
            if (mergeableUpper){
                trapezoidSecondEndpoint->getUpperLeftTrapezoid()->setUpperRightTrapezoid(B2);
            } else if (mergeableLower){
                trapezoidSecondEndpoint->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A2);
            }
        }
    }


    // DAG nodes building for right endpoint
    dag::XNode *xNode2 = new dag::XNode(rightMostId);
    dag::YNode *yNode2 = new dag::YNode(segmentId);
    dag::Leaf *A2l;
    dag::Leaf *B2l = new dag::Leaf(B2);
    dag::Leaf *C2l;

    // Create a new DAG leaf only if the trapezoid is not already inserted
    if (C2->getLeaf() == nullptr){
        C2l = new dag::Leaf(C2);
    } else {
        C2l = C2->getLeaf();
    }

    if (A2->getLeaf() == nullptr){
        A2l = new dag::Leaf(A2);
    } else {
        A2l = A2->getLeaf();
    }

    xNode2->setRightChild(B2l);
    xNode2->setLeftChild(yNode2);

    yNode2->setRightChild(C2l);
    yNode2->setLeftChild(A2l);

    // Find the node in the DAG and replace it with the new xNode
    dag::Node *deleteLeaf3 = trapezoidSecondEndpoint->getLeaf();

    assert(deleteLeaf3 != nullptr);

    // Find where deleteLeaf3 is a child and substitute it
    dag.updateChildren(deleteLeaf3, xNode2);

    xNode2->getFathers() = deleteLeaf3->getFathers();
    yNode2->getFathers().insert(xNode2);

    A2l->getFathers().insert(yNode2);
    B2l->getFathers().insert(xNode2);
    C2l->getFathers().insert(yNode2);

    A2->setLeaf(A2l);
    B2->setLeaf(B2l);
    C2->setLeaf(C2l);

    dag.deleteNode(deleteLeaf3);
    deleteLeaf3 = nullptr;
}

/**
 * @brief Find the list of trapezoids intersected by a segment
 * @param[in] dag
 * @param[in] segment
 * @param[in] trapezoidalMap
 * @return a set of intersected trapezoids
 */
std::vector<tmap::Trapezoid*> tbuild::followSegment(const dag::Dag &dag,
                                                    const cg3::Segment2d &segment,
                                                    const tmap::TrapezoidalMap &trapezoidalMap)
{
    std::vector<tmap::Trapezoid*> trapezoidList;
    int j = 0;

    cg3::Point2d leftMost = segment.p1();
    cg3::Point2d rightMost = segment.p2();

    // Re-order the segment points, p1 leftmost and p2 rightmost
    tmap::orderPoint(leftMost, rightMost);

    // Get trapezoid from DAG
    dag::Leaf *l = dag.findPoint(dag.getRoot(), trapezoidalMap, cg3::Segment2d(leftMost, rightMost));
    tmap::Trapezoid *trapezoid = l->getTrapezoid();

    trapezoidList.push_back(trapezoid);

    // If segment.p2 lies to the right of rightp
    while (rightMost.x() > trapezoidalMap.getPoint(trapezoidList[j]->getRightp()).x()) {

        // If rightp lies above the segment
        if (tmap::findPointSide(segment, trapezoidalMap.getPoint(trapezoidList[j]->getRightp()))){
            trapezoidList.push_back(trapezoidList[j]->getLowerRightTrapezoid());
        } else {
            trapezoidList.push_back(trapezoidList[j]->getUpperRightTrapezoid());
        }

        j++;
    }

    return trapezoidList;
}

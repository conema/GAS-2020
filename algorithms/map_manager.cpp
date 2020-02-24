#include "map_manager.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tbuild::buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset, dag::Dag &dag)
{
    // Initialize trapezoidalmap and dag, it cannot be done in the constructor because in the manager, the dataset is deleted after the map
    if (trapezoidalMap.getTrapezoids().size() == 0){
        trapezoidalMap.initializeTrapezoids(trapezoidalMapDataset);
        dag.initializeDag(new dag::Leaf(*trapezoidalMap.getTrapezoids().begin()));
    }

    // Find trapezoid intersected by the new segment
    std::vector<tmap::Trapezoid*> tl = tbuild::followSegment(dag, segment, trapezoidalMapDataset);

    cg3::Point2d leftMost = segment.p1();
    cg3::Point2d rightMost = segment.p2();

    // Re-order the segment points, p1 leftmost and p2 rightmost
    tmap::orderPoint(leftMost, rightMost);

    bool success;

    const size_t leftMostId = trapezoidalMapDataset.findPoint(leftMost, success);
    const size_t rightMostId = trapezoidalMapDataset.findPoint(rightMost, success);
    const size_t segmentId = trapezoidalMapDataset.findSegment(segment, success);

    if (tl.size() == 1) {
        // 1 trapezoid intersected
        tmap::Trapezoid *trapezoid = *tl.begin();

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

        // Update neighbors' neighbors
        if (trapezoid->getLowerLeftTrapezoid() != nullptr){
            trapezoid->getLowerLeftTrapezoid()->setLowerRightTrapezoid(A);
            trapezoid->getLowerLeftTrapezoid()->setUpperRightTrapezoid(A);
        }

        if (trapezoid->getUpperLeftTrapezoid() != nullptr){
            trapezoid->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A);
            trapezoid->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A);
        }

        if (trapezoid->getUpperRightTrapezoid() != nullptr){
            trapezoid->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B);
            trapezoid->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B);
        }

        if (trapezoid->getLowerRightTrapezoid() != nullptr){
            trapezoid->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B);
            trapezoid->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B);
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
        dag::Leaf *Al = new dag::Leaf(A);
        dag::Leaf *Bl = new dag::Leaf(B);
        dag::Leaf *Cl = new dag::Leaf(C);
        dag::Leaf *Dl = new dag::Leaf(D);

        // Find node on the DAG
        dag::Node *foundNode = trapezoid->getLeaf();

        // Find if findNode is a left or right child
        std::unordered_set<dag::Node*> findNodeFathers = foundNode->getFathers();

        if (findNodeFathers.size() == 0){
            // The foundNode is the root and it's substituted with si
            dag.setRoot(pi);
        }else {
            for (const auto& father: findNodeFathers){
                if (father->getLeftChild() == foundNode){
                    // The foundNode is a left child
                    father->setLeftChild(pi);
                } else {
                    // The foundNode is a right child
                    father->setRightChild(pi);
                }
            }
        }
        Al->getFathers().insert(pi);
        Bl->getFathers().insert(qi);
        Cl->getFathers().insert(si);
        Dl->getFathers().insert(si);

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
        delete foundNode;
    } else {
        // >1 Trapezoid intersected

        tmap::Trapezoid *trapezoidFirstEndpoint = *tl.begin();
        tmap::Trapezoid *trapezoidSecondEndpoint = tl.back();

        /* -----  TrapezoidalMap building  -----  */
        // 3 new trapezoids for the left endpoint of the segment
        // Left trapezoid
        tmap::Trapezoid *A1 = new tmap::Trapezoid(trapezoidFirstEndpoint->getTop(),
                                                 trapezoidFirstEndpoint->getBottom(),
                                                 leftMostId,
                                                 trapezoidFirstEndpoint->getLeftp());

        // Top trapezoid
        tmap::Trapezoid *B1 = new tmap::Trapezoid(trapezoidFirstEndpoint->getTop(),
                                                 segmentId,
                                                 trapezoidFirstEndpoint->getRightp(),
                                                 leftMostId);

        // Bottom trapezoid
        tmap::Trapezoid *C1 = new tmap::Trapezoid(segmentId,
                                                 trapezoidFirstEndpoint->getBottom(),
                                                 trapezoidFirstEndpoint->getRightp(),
                                                 leftMostId );

        // Add adjacencies
        A1->Trapezoid::updateAdjacencies(trapezoidFirstEndpoint->getLowerLeftTrapezoid(),
                                        trapezoidFirstEndpoint->getUpperLeftTrapezoid(),
                                        B1,
                                        C1);

        B1->Trapezoid::updateAdjacencies(A1,
                                        A1,
                                        nullptr,
                                        nullptr);

        C1->Trapezoid::updateAdjacencies(A1,
                                        A1,
                                        nullptr,
                                        nullptr);

        // Update neighbors' neighbors
        if (trapezoidFirstEndpoint->getLowerLeftTrapezoid() != nullptr){
            trapezoidFirstEndpoint->getLowerLeftTrapezoid()->setLowerRightTrapezoid(A1);
            trapezoidFirstEndpoint->getLowerLeftTrapezoid()->setUpperRightTrapezoid(A1);
        }

        if (trapezoidFirstEndpoint->getUpperLeftTrapezoid() != nullptr){
            trapezoidFirstEndpoint->getUpperLeftTrapezoid()->setLowerRightTrapezoid(A1);
            trapezoidFirstEndpoint->getUpperLeftTrapezoid()->setUpperRightTrapezoid(A1);
        }

        trapezoidalMap.removeTrapezoid(trapezoidFirstEndpoint);

        trapezoidalMap.addTrapezoid(A1);
        trapezoidalMap.addTrapezoid(B1);
        trapezoidalMap.addTrapezoid(C1);

        // Other trapezoid in the middle of the segment from t1+1 to tn-1
        tmap::Trapezoid *oldUpper = B1;
        tmap::Trapezoid *oldLower = C1;

        for (auto it = tl.begin()+1; it != tl.end()-1; ++it){
            tmap::Trapezoid* A3 = new tmap::Trapezoid((*it)->getTop(),
                                                      segmentId,
                                                      (*it)->getRightp(),
                                                      (*it)->getLeftp());

            tmap::Trapezoid* B3 = new tmap::Trapezoid(segmentId,
                                                      (*it)->getBottom(),
                                                      (*it)->getRightp(),
                                                      (*it)->getLeftp());



            A3->Trapezoid::updateAdjacencies(oldUpper, oldUpper, nullptr, nullptr);
            B3->Trapezoid::updateAdjacencies(oldLower, oldLower, nullptr, nullptr);

            oldUpper->setUpperRightTrapezoid(A3);
            oldUpper->setLowerRightTrapezoid(A3);

            oldLower->setUpperRightTrapezoid(B3);
            oldLower->setLowerRightTrapezoid(B3);

            oldUpper = A3;
            oldLower = B3;

            trapezoidalMap.addTrapezoid(A3);
            trapezoidalMap.addTrapezoid(B3);

            trapezoidalMap.removeTrapezoid(*it);
        }

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

        B2->Trapezoid::updateAdjacencies(A2,
                                        C2,
                                        trapezoidSecondEndpoint->getUpperRightTrapezoid(),
                                        trapezoidSecondEndpoint->getLowerRightTrapezoid());

        C2->Trapezoid::updateAdjacencies(oldLower,
                                        oldLower,
                                        B2,
                                        B2);

        oldUpper->setUpperRightTrapezoid(A2);
        oldUpper->setLowerRightTrapezoid(A2);

        oldLower->setUpperRightTrapezoid(C2);
        oldLower->setLowerRightTrapezoid(C2);

        // Update neighbors' neighbors
        if (trapezoidSecondEndpoint->getLowerRightTrapezoid() != nullptr){
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B2);
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }

        if (trapezoidSecondEndpoint->getUpperRightTrapezoid() != nullptr){
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B2);
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }

        trapezoidalMap.addTrapezoid(A2);
        trapezoidalMap.addTrapezoid(B2);
        trapezoidalMap.addTrapezoid(C2);

        trapezoidalMap.removeTrapezoid(trapezoidSecondEndpoint);


        /* ----- DAG nodes building -----  */
    }
}

/**
 * @brief Find the list of trapezoids intersected by a segment
 * @param[in] trapezoidalMap
 * @param[in] dag
 * @param[in] segment
 * @return a set of intersected trapezoids
 */
std::vector<tmap::Trapezoid*> tbuild::followSegment(const dag::Dag &dag,
                                                    const cg3::Segment2d &segment,
                                                    TrapezoidalMapDataset &trapezoidalMapDataset)
{
    bool found = false;
    std::vector<tmap::Trapezoid*> trapezoidList;
    int j = 0;

    cg3::Point2d leftMost = segment.p1();
    cg3::Point2d rightMost = segment.p2();

    // Re-order the segment points, p1 leftmost and p2 rightmost
    tmap::orderPoint(leftMost, rightMost);

    // Get trapezoid from DAG
    dag::Leaf *l = dag.findPoint(dag.getRoot(), trapezoidalMapDataset, trapezoidalMapDataset.findPoint(leftMost, found));
    tmap::Trapezoid *trapezoid = l->getTrapezoid();

    trapezoidList.push_back(trapezoid);

    // If segment.p2 lies to the right of rightp
    while (rightMost.x() > trapezoidalMapDataset.getPoint(trapezoidList[j]->getRightp()).x()) {

        // If rightp lies above the segment
        if (tmap::findPointSide(segment, trapezoidalMapDataset.getPoint(trapezoidList[j]->getRightp()).x())){
            trapezoidList.push_back(trapezoidList[j]->getLowerRightTrapezoid());
        } else {
            trapezoidList.push_back(trapezoidList[j]->getUpperRightTrapezoid());
        }

        j++;
    }

    return trapezoidList;
}

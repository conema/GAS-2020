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

        /* -----  Left endpoint  -----  */
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

        trapezoidalMap.addTrapezoid(A1);
        trapezoidalMap.addTrapezoid(B1);
        trapezoidalMap.addTrapezoid(C1);

        // DAG nodes building for left endpoint
        dag::XNode *xNode1 = new dag::XNode(leftMostId);
        dag::YNode *yNode1 = new dag::YNode(segmentId);
        dag::Leaf *A1l = new dag::Leaf(A1);
        dag::Leaf *B1l = new dag::Leaf(B1);
        dag::Leaf *C1l = new dag::Leaf(C1);

        xNode1->setLeftChild(A1l);
        xNode1->setRightChild(yNode1);

        yNode1->setLeftChild(B1l);
        yNode1->setRightChild(C1l);

        // Find the node in the DAG and replace it with the new xNode

        // TODO: refactoring con funzione
        dag::Leaf *deleteLeaf = trapezoidFirstEndpoint->getLeaf();
        dag::Node *father = *deleteLeaf->getFathers().begin();

        if (father->getLeftChild() == deleteLeaf){
            father->setLeftChild(xNode1);
        } else if (father->getRightChild() == deleteLeaf) {
           father->setRightChild(xNode1);
        }

        xNode1->getFathers().insert(father);
        yNode1->getFathers().insert(xNode1);
        C1l->getFathers().insert(xNode1);

        A1l->getFathers().insert(yNode1);
        B1l->getFathers().insert(yNode1);

        delete deleteLeaf;
        trapezoidalMap.removeTrapezoid(trapezoidFirstEndpoint);


        /* -----  Middle of the segments  -----  */
        // Trapezoid in the middle of the segment from tl(1+1) to tl(n-1)
        tmap::Trapezoid *oldUpper = B1;
        tmap::Trapezoid *oldLower = C1;

        bool mergeableUpper, mergeableLower;

        tmap::Trapezoid *A3, *B3;

        for (auto it = tl.begin()+1; it != tl.end()-1; ++it){
            mergeableUpper = (oldUpper->getTop() == (*it)->getTop() && oldUpper->getBottom() == segmentId);
            mergeableLower = (oldLower->getTop() == segmentId && oldLower->getBottom() == (*it)->getBottom());

            // Merge trapezoid
            if (mergeableUpper && mergeableLower){
                // Merge upper and lower
                oldUpper->setRightp((*it)->getRightp());
                oldLower->setRightp((*it)->getRightp());

                A3 = oldUpper;
                B3 = oldLower;
            } else if (mergeableUpper){
                // Merge upper trapezoid
                oldUpper->setRightp((*it)->getRightp());

                A3 = oldUpper;

                B3 = new tmap::Trapezoid(segmentId,
                                        (*it)->getBottom(),
                                        (*it)->getRightp(),
                                        (*it)->getLeftp());

                B3->Trapezoid::updateAdjacencies(oldLower, oldLower, nullptr, nullptr);

                oldLower->setUpperRightTrapezoid(B3);
                oldLower->setLowerRightTrapezoid(B3);

                oldLower = B3;
                trapezoidalMap.addTrapezoid(B3);

            } else if (mergeableLower){
                // Merge lower trapezoid
                oldLower->setRightp((*it)->getRightp());

                B3 = oldLower;

                A3 = new tmap::Trapezoid((*it)->getTop(),
                                         segmentId,
                                         (*it)->getRightp(),
                                         (*it)->getLeftp());

                A3->Trapezoid::updateAdjacencies(oldUpper, oldUpper, nullptr, nullptr);

                oldUpper->setUpperRightTrapezoid(A3);
                oldUpper->setLowerRightTrapezoid(A3);

                oldUpper = A3;

                trapezoidalMap.addTrapezoid(A3);

            } else {
                // No merging
                A3 = new tmap::Trapezoid((*it)->getTop(),
                                          segmentId,
                                         (*it)->getRightp(),
                                         (*it)->getLeftp());

                B3 = new tmap::Trapezoid(segmentId,
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
            }



            dag::Leaf *A3l = new dag::Leaf(A3);
            dag::Leaf *B3l = new dag::Leaf(B3);

            dag::YNode *yNode3 = new dag::YNode(segmentId);
            yNode3->setRightChild(B3l);
            yNode3->setLeftChild(A3l);

            dag::Node *deleteLeaf2 = (*it)->getLeaf();

            for (dag::Node* father: deleteLeaf2->getFathers()){
                if (father->getLeftChild() == deleteLeaf2){
                    father->setLeftChild(yNode3);
                } else if (father->getRightChild() == deleteLeaf2) {
                    father->setRightChild(yNode3);
                }
            }

            A3l->getFathers().insert(yNode3);
            B3l->getFathers().insert(yNode3);

            delete deleteLeaf2;

            trapezoidalMap.removeTrapezoid(*it);
        }



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

        mergeableUpper = (oldUpper->getTop() == trapezoidSecondEndpoint->getTop() && oldUpper->getBottom() == segmentId);
        mergeableLower = (oldLower->getTop() == segmentId && oldLower->getBottom() == trapezoidSecondEndpoint->getBottom());

        if (mergeableUpper && mergeableLower){
            // Merge upper and lower
            oldUpper->setRightp(rightMostId);
            oldLower->setRightp(rightMostId);

            delete A2;
            delete C2;

            A2 = oldUpper;
            C2 = oldLower;

            A2->Trapezoid::updateAdjacencies(A2->getLowerLeftTrapezoid(),
                                             A2->getUpperLeftTrapezoid(),
                                             B2,
                                             B2);

            B2->Trapezoid::updateAdjacencies(A2,
                                            C2,
                                            trapezoidSecondEndpoint->getUpperRightTrapezoid(),
                                            trapezoidSecondEndpoint->getLowerRightTrapezoid());

            C2->Trapezoid::updateAdjacencies(C2->getLowerLeftTrapezoid(),
                                             C2->getUpperLeftTrapezoid(),
                                             B2,
                                             B2);
        } else if (mergeableUpper){
            // Merge upper trapezoid
            oldUpper->setRightp(rightMostId);

            delete A2;
            A2 = oldUpper;

            A2->Trapezoid::updateAdjacencies(A2->getLowerLeftTrapezoid(),
                                             A2->getUpperLeftTrapezoid(),
                                             B2,
                                             B2);

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

            trapezoidalMap.addTrapezoid(A2);
        } else {
            // No merging
            trapezoidalMap.addTrapezoid(A2);
            trapezoidalMap.addTrapezoid(C2);

            oldUpper->setUpperRightTrapezoid(A2);
            oldUpper->setLowerRightTrapezoid(A2);
            oldLower->setUpperRightTrapezoid(C2);
            oldLower->setLowerRightTrapezoid(C2);
        }

        trapezoidalMap.addTrapezoid(B2);


        // Update neighbors' neighbors
        if (trapezoidSecondEndpoint->getLowerRightTrapezoid() != nullptr){
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setLowerLeftTrapezoid(B2);
            trapezoidSecondEndpoint->getLowerRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }

        if (trapezoidSecondEndpoint->getUpperRightTrapezoid() != nullptr){
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setLowerLeftTrapezoid(B2);
            trapezoidSecondEndpoint->getUpperRightTrapezoid()->setUpperLeftTrapezoid(B2);
        }


        // DAG nodes building for right endpoint
        dag::XNode *xNode2 = new dag::XNode(rightMostId);
        dag::YNode *yNode2 = new dag::YNode(segmentId);
        dag::Leaf *A2l = new dag::Leaf(A2);
        dag::Leaf *B2l = new dag::Leaf(B2);
        dag::Leaf *C2l = new dag::Leaf(C2);

        xNode2->setRightChild(B2l);
        xNode2->setLeftChild(yNode2);

        yNode2->setRightChild(C2l);
        yNode2->setLeftChild(A2l);

        // Find the node in the DAG and replace it with the new xNode
        dag::Node *deleteLeaf3 = trapezoidSecondEndpoint->getLeaf();
        dag::Node *father2 = *deleteLeaf3->getFathers().begin();

        if (father2->getLeftChild() == deleteLeaf3){
            father2->setLeftChild(xNode2);
        } else if (father2->getRightChild() == deleteLeaf3) {
            father2->setRightChild(xNode2);
        }

        xNode2->getFathers().insert(father2);
        yNode2->getFathers().insert(xNode2);
        C2l->getFathers().insert(xNode2);

        A2l->getFathers().insert(yNode2);
        B2l->getFathers().insert(yNode2);


        delete deleteLeaf3;
        trapezoidalMap.removeTrapezoid(trapezoidSecondEndpoint);
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
    std::vector<tmap::Trapezoid*> trapezoidList;
    int j = 0;

    cg3::Point2d leftMost = segment.p1();
    cg3::Point2d rightMost = segment.p2();

    // Re-order the segment points, p1 leftmost and p2 rightmost
    tmap::orderPoint(leftMost, rightMost);

    // Get trapezoid from DAG
    dag::Leaf *l = dag.findPoint(dag.getRoot(), trapezoidalMapDataset, leftMost);
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

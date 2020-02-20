#include "map_manager.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tbuild::buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset, dag::Dag &dag)
{
    /* -----  TrapezoidalMap building  -----  */

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

    // 1 trapezoid intersected
    if (tl.size() == 1) {
        tmap::Trapezoid *trapezoid = *tl.begin();

        // For new trapezoid for the new segment
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

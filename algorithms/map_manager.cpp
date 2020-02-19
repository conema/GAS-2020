#include "map_manager.h"

/**
 * @brief Insert a new segment into the TrapezoidalMap and the DAG
 * @param[in] segment: segment
 */
void tbuild::buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset, dag::Dag &dag)
{
    /* -----  TrapezoidalMap building  -----  */
    // TODO: da cambiare
    tmap::Trapezoid *oldTrapezoid = *trapezoidalMap.getTrapezoids().begin();

    // p1 leftmost and p2 rightmost
    const cg3::Point2d &leftMost = segment.p1().min(segment.p2());
    const cg3::Point2d &rightMost = segment.p1().max(segment.p2());;

    bool success;

    const size_t leftMostId = trapezoidalMapDataset.findPoint(leftMost, success);
    const size_t rightMostId = trapezoidalMapDataset.findPoint(rightMost, success);
    const size_t segmentId = trapezoidalMapDataset.findSegment(segment, success);

    // For new trapezoid for the new segment
    tmap::Trapezoid *A = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      oldTrapezoid->getBottom(),
                      leftMostId,
                      oldTrapezoid->getLeftp());

    tmap::Trapezoid *B = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      oldTrapezoid->getBottom(),
                      oldTrapezoid->getRightp(),
                      rightMostId);

    tmap::Trapezoid *C = new tmap::Trapezoid(oldTrapezoid->getTop(),
                      segmentId,
                      rightMostId,
                      leftMostId);

    tmap::Trapezoid *D = new tmap::Trapezoid(segmentId,
                      oldTrapezoid->getBottom(),
                      rightMostId,
                      leftMostId);

    // Add adjacencies
    A->Trapezoid::updateAdjacencies(oldTrapezoid->getLowerLeftTrapezoid(),
                                   oldTrapezoid->getUpperLeftTrapezoid(),
                                   C,
                                   D);

    B->Trapezoid::updateAdjacencies(D,
                                   C,
                                   oldTrapezoid->getUpperRightTrapezoid(),
                                   oldTrapezoid->getLowerRightTrapezoid());
    C->Trapezoid::updateAdjacencies(A,
                                   A,
                                   B,
                                   B);
    D->Trapezoid::updateAdjacencies(A,
                                   A,
                                   B,
                                   B);

    trapezoidalMap.removeTrapezoid(oldTrapezoid);
    trapezoidalMap.addTrapezoid(A);
    trapezoidalMap.addTrapezoid(B);
    trapezoidalMap.addTrapezoid(C);
    trapezoidalMap.addTrapezoid(D);

    /* ----- DAG nodes building -----  */
    //Creatore x-nodes and y-nodes
    dag::XNode *pi = new dag::XNode(leftMostId);
    dag::XNode *qi = new dag::XNode(rightMostId);
    dag::YNode *si = new dag::YNode(segmentId);

    // Create leaves with trapezoids
    dag::Leaf *Al = new dag::Leaf(A);
    dag::Leaf *Bl = new dag::Leaf(B);
    dag::Leaf *Cl = new dag::Leaf(C);
    dag::Leaf *Dl = new dag::Leaf(D);

    pi->setLeftChild(Al);
    pi->setRightChild(qi);

    qi->setLeftChild(si);
    qi->setRightChild(Bl);

    si->setLeftChild(Cl);
    si->setRightChild(Dl);

    // Link between trapezoids and leaves
    A->setLeaf(Al);
    B->setLeaf(Bl);
    C->setLeaf(Cl);
    D->setLeaf(Dl);

    dag.setRoot(pi);
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

    // Get trapezoid from DAG
    dag::Leaf *l = dag.findPoint(dag.getRoot(), trapezoidalMapDataset, trapezoidalMapDataset.findPoint(segment.p1(), found));
    tmap::Trapezoid *trapezoid = l->getTrapezoid();

    trapezoidList.push_back(trapezoid);

    // If segment.p2 lies to the right of rightp
    while (segment.p2().x() > trapezoidalMapDataset.getPoint(trapezoidList[j]->getRightp()).x()) {

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

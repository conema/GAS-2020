#include "dag.h"

dag::Dag::Dag(dag::Leaf *node):
    root(node)
{
    node->getTrapezoid()->setLeaf(node);
}

void dag::Dag::setRoot(Node *value)
{
    root = value;
}

dag::Node *dag::Dag::getRoot() const
{
    return root;
}

/*dag::Node *dag::Dag::findNode(const Node &node)
{
    if (root) {

    }
}*/

/*void removeLeaf(dag::Leaf &leaf){

}*/

#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <data_structures/tmap/trapezoidalmap.h>
#include <data_structures/trapezoidalmap_dataset.h>
#include <data_structures/dag/dag.h>
#include <data_structures/dag/node.h>
#include <data_structures/dag/x_node.h>
#include <data_structures/dag/y_node.h>
#include <data_structures/dag/leaf.h>

namespace tmap {
    void buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset, dag::Dag &Dag);
}

#endif // MAP_MANAGER_H

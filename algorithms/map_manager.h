#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <data_structures/tmap/trapezoidalmap.h>
#include <data_structures/trapezoidalmap_dataset.h>

namespace tmap {
    void buildMap(const cg3::Segment2d &segment, tmap::TrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapDataset);
}

#endif // MAP_MANAGER_H

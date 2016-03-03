#ifndef PRINTABLE_MESH_LAYER_PART_H
#define PRINTABLE_MESH_LAYER_PART_H

#include "PrintableLayerPart.h"

namespace cura
{

class PrintableMeshLayerPart : public PrintableLayerPart
{
public:
    Polygons& getOutline();
    AABB getBoundaryBox() = 0;
    int64_t getZ() = 0;
    void writeGCode() = 0;
private:
    unsigned int mesh_idx;
    unsigned int layer_idx;
    unsigned int part_idx;
};

} // namespace cura

#endif // PRINTABLE_MESH_LAYER_PART_H
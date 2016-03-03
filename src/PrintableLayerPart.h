#ifndef PRINTABLE_LAYER_PART_H
#define PRINTABLE_LAYER_PART_H

#include "utils/polygon.h"
#include "utils/AABB.h"
#include "sliceDataStorage.h"

namespace cura
{

/*!
 * Abstract class for representing a printable part within a layer.
 * This could be a SliceLayerPart of a mesh, a piece of support, a prime tower, etc.
 * 
 * The PrintableLayerPart knows itself how it should generate its paths.
 */
class PrintableLayerPart
{
public:
    virtual Polygons& getOutline() = 0;
    virtual AABB getBoundaryBox() = 0;
    virtual int64_t getZ() = 0;
    void generatePaths();
    bool isGenerated();
protected:
    SliceDataStorage& storage;
    bool is_generated;
    
    virtual void writePaths();
    
    
    PrintableLayerPart(SliceDataStorage& storage);
    
};


} // namespace cura

#endif // PRINTABLE_LAYER_PART_H
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
    void generatePaths(GCodePlanner& layer_plan);
    bool isGenerated();
    int getExtruderNr();
    int getLayerNr();
protected:
    SliceDataStorage& storage;
    bool is_generated;
    int extruder_nr;
    int layer_nr;
    
    
    virtual void writeGcode(GCodePlanner& layer_plan);
    
    
    PrintableLayerPart(SliceDataStorage& storage, int extruder_nr, int layer_nr)
    : storage(storage)
    , is_generated(false)
    , extruder_nr(extruder_nr)
    , layer_nr(layer_nr)
    {
    }
    
};


} // namespace cura

#endif // PRINTABLE_LAYER_PART_H
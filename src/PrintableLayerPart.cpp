#include "PrintableLayerPart.h"

namespace cura
{

int PrintableLayerPart::getExtruderNr()
{
    return extruder_nr;
}

int PrintableLayerPart::getLayerNr()
{
    return layer_nr;
}
    
bool PrintableLayerPart::isGenerated()
{
    return is_generated;
}

void PrintableLayerPart::generatePaths(GCodePlanner& layer_plan)
{
    if (is_generated)
    {
        logError("WTF this layer part is already written to gcode@!!!!!");
    }
    writeGCode(layer_plan);
    is_generated = true;
}

} // namespace cura
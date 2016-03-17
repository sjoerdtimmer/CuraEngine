#include "PrintableLayerPart.h"

namespace cura
{

unsigned int PrintableLayerPart::getExtruderNr()
{
    return extruder_nr;
}

unsigned int PrintableLayerPart::getLayerNr()
{
    return layer_nr;
}
    
bool PrintableLayerPart::isGenerated()
{
    return is_generated;
}

bool PrintableLayerPart::isPicked()
{
    return is_picked;
}

void PrintableLayerPart::pick()
{
    is_picked = true;
}


void PrintableLayerPart::generatePaths(GCodePlanner& layer_plan)
{
    if (is_generated)
    {
        logError("WTF this layer part is already written to gcode@!!!!!\n");
    }
    writeGcode(layer_plan);
    is_generated = true;
}

} // namespace cura
#include "PrintableLayerPart.h"

namespace cura
{


bool PrintableLayerPart::isGenerated()
{
    return is_generated;
}

void PrintableLayerPart::generatePaths()
{
    if (is_generated)
    {
        logError("WTF this layer part is already written to gcode@!!!!!");
    }
    GCodePlanner& layer_plan(storage, layer_nr, z, layer_height, last_position, current_extruder, fan_speed_layer_time_settings, retraction_combing, comb_boundary_offset, travel_avoid_other_parts, travel_avoid_distance);
    writeGcode(layer_plan);
    is_generated = true;
}

} // namespace cura
#include "PrintableLayerPart.h"

namespace cura
{

PrintableLayerPart::PrintableLayerPart(SliceDataStorage& storage)
: storage(storage)
, is_generated(false)
{
}

void cura::PrintableLayerPart::generatePaths()
{
    if (is_generated)
    {
        logError("WTF this layer part is already written to gcode@!!!!!");
    }
    writePaths();
    is_generated = true;
}

} // namespace cura
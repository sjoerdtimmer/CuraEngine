
#include "settings.h"
#include "PrintableLayer.h"
#include "PrintableLayerPart.h"
#include "Towering.h"

namespace cura {

    

Towering::Towering(std::vector<PrintableLayer>& layers)
{
//    PrintableLayerPart* last_processed;
    

    // the first layer is special because it 'sits' on nothing.
    // All following layers assume that at least one layerpart has been printed 
    assert(layers.size() >= 1);
    assert(layers.front().parts.size() >= 1);
    // TODO: start with the one closest to startpoint
//     last_processed = &(layers.front().parts.front());
    int64_t max_generated_z = layers.front().getZ();
    // TODO: maybe store the highest processed layernumber instead??
//     last_processed->generatePaths();
    
    Point last_extruder_location = Point(0,0);// TODO: figure out what the last extruder location was
    
    // TODO: get the layer height from somewhere
    // TODO: make maximum lookahead configurable
    int layer_height = 100;//?.getSettingInMicrons('layer_height');
    while( processNextPart(layers,max_generated_z + 3000 + layer_height, last_extruder_location) )
    {
	// ...
    }   
}




bool Towering::processNextPart(std::vector< PrintableLayer >& layers, int64_t max_z, Point last_extruder_location)
{
    // look for a candidate next layerpart that have overlap with last extruded point:
    for( PrintableLayer& candidatelayer : layers )
    {
	
	if (candidatelayer.getZ() > max_z ) break; // don't look too far ahead
	
	// look for parts in this candidatelayer
	for (PrintableLayerPart* candidatepart : candidatelayer.parts)
	{
	    if (candidatepart->isGenerated()) continue; // parts that have already been generated cannot be generated again
	    if (layerPartCanBePrintedNext(*candidatepart, layers) && candidatepart->getOutline().inside(last_extruder_location))
	    {
                candidatepart->generatePaths();
		return true;
	    }
	}
    }
    
    
    
    // no good part found: fall back to simply the next
    log("tower finished, falling back to any other part\n");
    for(PrintableLayer& candidatelayer : layers)
    {
	for(PrintableLayerPart* candidatepart : candidatelayer.parts){
	    if( ! candidatepart->isGenerated())
	    {
		candidatepart->generatePaths();
		return true;
	    }
	}
    }
    
    
    // if all parts have been generated
    log("all parts have been generated\n");
    return false;
}



// whether layer1 blocks printing layer2
bool Towering::partBlocksOtherPart(PrintableLayerPart& part1, PrintableLayerPart& part2)
{
    // some quick heuristics to save time:
    if(part1.getZ() <= part2.getZ()) return false;       // lower layers never block higher layers
    if(part2.getZ() - part2.getZ() > 3000) return true;  // never print more than 3mm before catching up. 
    
    Polygons& outline1 = part1.getOutline();
    Polygons& outline2 = part2.getOutline();
    
    Polygons keepoutarea = outline2.offset(3000+part1.getZ()-part2.getZ(),ClipperLib::jtRound);
    
    if(keepoutarea.intersection(outline1).size() > 0)
    {
	return true;
    }
    return false;  
}


double Towering::layerPartOverlapArea(PrintableLayerPart& part1, PrintableLayerPart& part2)
{
    double area = 0;
    for(PolygonRef polygon : part1.getOutline().intersection(part2.getOutline()))
    {
	area += polygon.area();
    }
    return area;
}


bool Towering::layerPartCanBePrintedNext(PrintableLayerPart& part, std::vector<PrintableLayer>& layers)
{
    for(PrintableLayer& otherlayer : layers)
    {
	for(PrintableLayerPart* otherpart : otherlayer.parts)
	{
	    if(otherpart->isGenerated()) continue; // already printed parts are never a reason not to print the current candidate
	    // TODO: make sure that this is a valid way to check if part and otherpart are the same
	    if(&part == otherpart) continue;
	    if(partBlocksOtherPart(part,*otherpart)) return false;
	}
    }
    return true;
}

}
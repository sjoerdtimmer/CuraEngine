/* 
 * File:   Towering.h
 * Author: sjoerd
 *
 * Created on March 3, 2016, 8:55 PM
 */

#ifndef TOWERING_H
#define TOWERING_H

//#include <vector>
//#include "PrintableLayerPart.h"
#include "PrintableLayer.h"
//#include "mesh.h"

namespace cura
{

class Towering
{
public:
    /*! Find the best order to process layerpart. This includes finding towers. This function calls generatePaths() exactly once for every part in every layer that is passed in.
     *  \param layers the list of layers of parts
     */
    void processPrintableLayers(std::vector<PrintableLayer>& layers);
    
private:
    
    
    /*! Calculate the overlap area between two parts. This is no longer used. Was used to estimate the 'fitness' of the next part given the pevious part. Currently we simply check that the last_extruder_location is inside the part.
     * \param part1 first part
     * \param part2 second part
     * 
     * \return area of the intersection of the parts
     */
    double layerPartOverlapArea(PrintableLayerPart& part1, PrintableLayerPart& part2);
    
    
    /*! Determines whether printing part1 now will cause problems with printing part2 later. I.e. whether part2 hangs over or touches part1.
     * \param part1 part to be printed now
     * \param part2 part to be printed later
     * \return whether this order of printing causes problems
     */
    bool partBlocksOtherPart(PrintableLayerPart& part1, PrintableLayerPart& part2);
    
    
    /*! Determines whether a part can be printed without obstructing future parts.
     * \param part the part that is to be printed
     * \param layers the collection of layers to check for collisions
     * \return whether this part can be printed now.
     */
    bool layerPartCanBePrintedNext(PrintableLayerPart& part,std::vector<PrintableLayer>& layers);
    
    
    /*! From the list of layers, find a layerpart that fits nicely on to of the last extruded location.
     * \param layers list of PrintableLayers too choose from.
     * \param max_z pick a layerpart that is not above this value. used for speeding up the process.
     * \param last_extruder_location location of the extruder.
     * 
     * \return boolean, whether a part was found
     */
    bool processNextPart(std::vector< PrintableLayer >& layers, int64_t max_z, Point last_extruder_location);
};

}

#endif /* TOWERING_H */


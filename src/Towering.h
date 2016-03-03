/* 
 * File:   Towering.h
 * Author: sjoerd
 *
 * Created on March 3, 2016, 8:55 PM
 */

#ifndef TOWERING_H
#define TOWERING_H

#include <vector>
#include "PrintableLayerPart.h"
#include "PrintableLayer.h"
#include "mesh.h"

namespace cura
{

class Towering{
public:
    processPrintableLayers(std::vector<PrintableLayer> layers);
};
    
}

#endif /* TOWERING_H */


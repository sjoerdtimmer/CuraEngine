
#include "MeshGroup.h"
#include "settings.h"
#include "sliceDataStorage.h"



namespace cura {

void Towering::processPrintableLayers(std::vector<PrintableLayer> layers)
{
//    
//    // first count the number of layerparts so we know how big the vector of new layers needs to be:
//    unsigned int numparts = 0;
//    for (SliceLayer layer : mesh.layers) {
//        numparts += layer.parts.size();
//    }
//    // we store the SliceLayerParts each in their own SliceLayers, because otherwise the z value would be lost
//    std::vector<SliceLayer> partiallayers;
//    partiallayers.reserve(numparts);
//
//    for (SliceLayer layer : mesh.layers) {
//        for (SliceLayerPart layerpart : layer.parts) {
//            // use emplace to create a completely novel SliceLayer object
//            SliceLayer newlayer = SliceLayer();
//            //                    partiallayers.emplace(partiallayers.end()).base();
//            newlayer.printZ = layer.printZ;
//            newlayer.sliceZ = layer.sliceZ; // probably never used but for good measure copy it anyway
//            newlayer.parts.push_back(layerpart);
//            partiallayers.push_back(newlayer);
//        }
//    }
//
//    // now delete the old layer storage and replace it with the contents of the new list of layers
//    mesh.layers.clear();
//    mesh.layers.reserve(numparts); // resize to same size
//    for (SliceLayer& layer : partiallayers) {
//        mesh.layers.push_back(layer);
//    }
//
//    // next: reorder layer(part)s for optimal order
//    std::list<SliceLayer> toprocess; // all layerparts that need to be processed
//    std::list<SliceLayer> processed; // the ordered list of items in the best order
//
//    // initially everything has to be processed
//    for (SliceLayer& layer : mesh.layers) {
//        toprocess.push_back(layer);
//        //        log("new layer has %d outline polygons\n",layer.parts.back().outline.size());
//    }
//
//    //    log("toprocess layer order:\n");
//    //    for(SliceLayer& layer : toprocess){
//    //        log("  %d\n",layer.printZ);
//    //    }
//    //    
//
//    // the first layer is special because it 'sits' on nothing such that it has align to
//    assert(toprocess.size() >= 1);
//
//    processed.push_front(toprocess.front());
//    int maximum_height_of_processed_layers = toprocess.front().printZ;
//    toprocess.pop_front(); // weird that this returns void :s
//
//
//
//    while (toprocess.size() > 0) {
//        // find the unprocessed layerpart that 
//        // 1) does not block any other unprocessed layerpart
//        // 2) future: optimisations are possible, for instance: stop looking when the inspected unprocessed layerpart is more than layerhight above the highest processed layer...
//        // 3) with maximum overlap with the lastprocessed layerpart
//        SliceLayer* best_candidate;
//        double best_candidate_overlap_area = 0;
//        bool foundone = false;
//        std::list<SliceLayer>::iterator candidate_iterator; // use an iterator so that we can store it for later to remove the appropriate element
//        std::list<SliceLayer>::iterator other_iterator;
//        std::list<SliceLayer>::iterator best_candidate_it;
//
//        for (candidate_iterator = toprocess.begin(); candidate_iterator != toprocess.end(); candidate_iterator++) {
//            SliceLayer& candidate = *candidate_iterator;
//            // stop early if this candidate is too far away:
//            if (candidate.printZ > maximum_height_of_processed_layers + meshgroup->getSettingInMicrons("layer_height")) continue;
//
//            bool candidate_is_appropriate = true; // i.e. doesn't block anything that is still to be printed later on
//            for (other_iterator = toprocess.begin(); other_iterator != toprocess.end() && candidate_is_appropriate; other_iterator++) {
//                SliceLayer& other = *other_iterator;
//                if (candidate_iterator == other_iterator) continue; // layers don't block themselves
//
//                if (layerBlocksOtherLayer(candidate, other)) {
//                    candidate_is_appropriate = false;
//                }
//            }
//            if (candidate_is_appropriate) {
//                // this is an applicable candidate !
//                // calculate overlap with last layer:
//                assert(candidate.parts.size() == 1);
//                assert(processed.back().parts.size() == 1);
//                double overlap = layerPartOverlapArea(candidate.parts.back(), processed.back().parts.back());
//                if (overlap > best_candidate_overlap_area) {
//                    foundone = true; // there is probably a better way to do this
//                    best_candidate = &candidate;
//                    best_candidate_overlap_area = overlap;
//                    best_candidate_it = candidate_iterator;
//                }
//            }
//        }
//        if (foundone) { // if at least one layer was indeed a good next layerpart
//            processed.push_back(*best_candidate);
//            toprocess.erase(best_candidate_it);
//            log("found a good next layerpart at z=%d\n", best_candidate->printZ);
//        } else { // no layerpart is good enough for me, simply take the next and admit defeat
//            // (i.e. a travel move will have to be made)
//            processed.push_back(toprocess.front());
//            log("no layer matches any more, falling back to z=%d\n", toprocess.front().printZ);
//            toprocess.pop_front();
//        }
//        maximum_height_of_processed_layers = std::max(maximum_height_of_processed_layers, processed.back().printZ);
//    }
//
//
//
//    // 'processed' is now a reordering of layers
//    // todo: make all of this layer hussling more efficient
//    mesh.layers.clear();
//    mesh.layers.reserve(numparts); // resize to same size again
//    //    log("new layer order:\n");
//    for (SliceLayer& layer : processed) {
//        //        log("  %d\n",layer.printZ);
//        mesh.layers.push_back(layer);
//    }
//
//
//
//
//
//    // this variable is used to truncate processing of layers above a certain 
//    // index to prevent extra work if some meshes are shorter than others:
//    mesh.layer_nr_max_filled_layer = numparts;

        
}

}
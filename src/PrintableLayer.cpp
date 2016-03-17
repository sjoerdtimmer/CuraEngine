#include "PrintableLayer.h"

namespace cura
{

PrintableLayer::PrintableLayer(SliceDataStorage& storage, unsigned int layer_nr)
{
    this->z = storage.meshes.back().layers.at(layer_nr).sliceZ;
    
    // TODO: think of an elegant way to create PrintableLayerParts in-place because emplace_back wont work because the layerparts are a virtual class
    // TODO: fix hardcoding 
    for (unsigned int mesh_idx = 0; mesh_idx < storage.meshes.size(); mesh_idx++)
    {
        SliceMeshStorage& mesh = storage.meshes[mesh_idx];
        SliceLayer& layer = mesh.layers[layer_nr];
        assert(layer.sliceZ == this->z);
        for (unsigned int part_idx = 0; part_idx < layer.parts.size(); part_idx++)
        {
            // TODO: fix memory leak:
            PrintableMeshLayerPart* newpart = new PrintableMeshLayerPart(storage, mesh_idx, layer_nr, part_idx);
            this->parts.push_back(newpart);
//            this->parts.push_back(part);
        }
    }
}

} // namespace cura
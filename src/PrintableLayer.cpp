#include "PrintableLayer.h"

namespace cura
{

PrintableLayer::PrintableLayer(SliceDataStorage& storage, unsigned int layer_nr, unsigned int total_layers)
{
    Progress::messageProgress(Progress::Stage::EXPORT, layer_nr+1, total_layers);
    
    int layer_thickness = getSettingInMicrons("layer_height");
    if (layer_nr == 0)
    {
	layer_thickness = getSettingInMicrons("layer_height_0");
    }
    
    int64_t comb_offset_from_outlines = storage.meshgroup->getExtruderTrain(current_extruder_planned)->getSettingInMicrons("machine_nozzle_size") * 2; // TODO: only used when there is no second wall.
    int64_t z = storage.meshes[0].layers[layer_nr].printZ;
    GCodePlanner& gcode_layer = layer_plan_buffer.emplace_back(storage, layer_nr, z, layer_thickness, last_position_planned, current_extruder_planned, fan_speed_layer_time_settings, getSettingBoolean("retraction_combing"), comb_offset_from_outlines, getSettingBoolean("travel_avoid_other_parts"), getSettingInMicrons("travel_avoid_distance"));
    
    if (layer_nr == 0)
    {
	int start_extruder = 0; // TODO: make settable
	gcode_layer.setExtruder(start_extruder);
	processSkirt(storage, gcode_layer, start_extruder);
    }
    
    int extruder_nr_before = gcode_layer.getExtruder();
    addSupportToGCode(storage, gcode_layer, layer_nr, extruder_nr_before, true);
    
    processOozeShield(storage, gcode_layer, layer_nr);
    
    processDraftShield(storage, gcode_layer, layer_nr);
    
    //Figure out in which order to print the meshes, do this by looking at the current extruder and preferer the meshes that use that extruder.
    std::vector<unsigned int> mesh_order = calculateMeshOrder(storage, gcode_layer.getExtruder());
    gcode_layer.setIsInside(true);
    for(unsigned int mesh_idx : mesh_order)
    {
	SliceMeshStorage* mesh = &storage.meshes[mesh_idx];
	if (mesh->getSettingAsSurfaceMode("magic_mesh_surface_mode") == ESurfaceMode::SURFACE)
	{
	    gcode_layer.setIsInside(false);
	    addMeshLayerToGCode_meshSurfaceMode(storage, mesh, gcode_layer, layer_nr);
	}
	else
	{
	    gcode_layer.setIsInside(true); // needed when the last mesh was spiralized
	    addMeshLayerToGCode(storage, mesh, gcode_layer, layer_nr);
	}
    }
    gcode_layer.setIsInside(false);
    
    addSupportToGCode(storage, gcode_layer, layer_nr, extruder_nr_before, false);
    
    { // add prime tower if it hasn't already been added
	// print the prime tower if it hasn't been printed yet
	int prev_extruder = gcode_layer.getExtruder(); // most likely the same extruder as we are extruding with now
	addPrimeTower(storage, gcode_layer, layer_nr, prev_extruder);
    }
    
    last_position_planned = gcode_layer.getLastPosition();
    current_extruder_planned = gcode_layer.getExtruder();
    
    gcode_layer.processFanSpeedAndMinimalLayerTime();
}

} // namespace cura
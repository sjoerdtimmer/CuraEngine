/** Copyright (C) 2015 Ultimaker - Released under terms of the AGPLv3 License */
#include "GcodeBuffer.h"


namespace cura 
{


void GcodeBuffer::estimate(TimeEstimateCalculator::Position newPos, double feedrate)
{
    TimeEstimateCalculator::Block block;
    
    if (estimateCalculator.plan(newPos, feedrate, block))
    {
        gcode_blocks.back()->time_block = block;
//         assert(gcode_blocks.back()->time_block.nominal_feedrate != 0.0);
        gcode_blocks.back()->finished = true;
        gcode_blocks.push_back(new GcodeBlock());
    }
}

void GcodeBuffer::estimate(double time)
{
    if (time > 0.0)
    {
        gcode_blocks.back()->time = time;
        gcode_blocks.back()->finished = true;
        gcode_blocks.push_back(new GcodeBlock());
    }
    // otherwise keep adding gcode to the same block
}

std::stringstream& GcodeBuffer::insert(double time, bool before)
{
    if (before)
    {
        double acc_time = 0.0;
        for (int pos = gcode_blocks.size() - 1; pos >= 0; pos--)
        {
            acc_time += gcode_blocks[pos]->time;
            if (acc_time >= time)
            {
                inserts.emplace_back(static_cast<unsigned int>(pos));
                return inserts.back();
            }
        }
        logWarning("Warning: trying to insert gcode before a time larger than the gcode buffer!");
        inserts.emplace_front(0);
        return inserts.front();
    }
    else 
    {
        double acc_time = 0.0;
        for (int pos = 0; pos < int(gcode_blocks.size()); pos++)
        {
            acc_time += gcode_blocks[pos]->time;
            if (acc_time >= time)
            {
                inserts.emplace_front(static_cast<unsigned int>(pos));
                return inserts.front();
            }
        }
        logWarning("Warning: trying to insert gcode after a time larger than the gcode buffer!");
        inserts.emplace_back(gcode_blocks.size());
        return inserts.back();
    }
}

void GcodeBuffer::flush()
{
    estimate(0.001); // flush last gcode block of the layer which wasn't estimated by a timeEstime.plan(.)
    gcode_blocks.pop_back(); // remove the last new empty gcode block
    
    totalPrintTime += estimateCalculator.calculate();
        
    inserts.sort([](const Insert& a, const Insert& b) -> bool { return a.getPos() < b.getPos(); } );
    for (unsigned int block_idx = 0; block_idx < gcode_blocks.size(); block_idx++)
    {
        while ( ! inserts.empty() && block_idx == inserts.front().getPos())
        { // insert all inserts which should be inserted at this position (in arbitrary order)
            *output_stream << inserts.front().str();
            inserts.pop_front();
        }
            
        GcodeBlock& block = *gcode_blocks[block_idx];
        *output_stream << block.str();
    }
    while ( ! inserts.empty())
    { // insert all inserts which should be inserted at the end (and all inserts which were accidentally skipped)
        *output_stream << inserts.front().str();
        inserts.pop_front();
    }
    
    for (GcodeBlock* block : gcode_blocks)
    {
        delete block;
    }
    
    gcode_blocks.clear();
    gcode_blocks.push_back(new GcodeBlock());
    inserts.clear();
    total_time = 0.0;
}
    
    
    
    
    
    
    
    
    
    
} // namespace cura 
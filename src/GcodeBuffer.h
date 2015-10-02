/** Copyright (C) 2015 Ultimaker - Released under terms of the AGPLv3 License */
#ifndef GCODE_BUFFER_H
#define GCODE_BUFFER_H

#include <sstream>
#include <vector>
#include <ostream>
#include <algorithm>
#include <list>
#include <iostream>

#include "utils/logoutput.h"

namespace cura 
{

    
     
/*!
 * A class used to buffer gcode before writing it.
 * 
 * It keeps track of time estimates, so that commands can later be inserted into the buffer.
 */
class GcodeBuffer 
{
    /*!
     * Block of gcode commands with the estimated time it takes to execute them.
     * 
     * A block is supposed to be a single command with the time it takes to execute it, but since some commands do not take time 
     * the block should consist of a couple of commands which are executed immmediately, 
     * followed by a single command which takes time (retraction or move).
     */
    class Block : public std::stringstream
    {
    public:
        double time; // estimation of the time it takes to print the gcode commands present in this block of gcode
        Block() 
        : time(0.0) 
        { 
            *this << std::fixed;
        }
    };
    
    class Insert : public std::stringstream
    {
        friend class GcodeBuffer;
        unsigned int pos; // position of the item in GcodeBuffer::gcode_blocks before which to insert gcode of this Insert
    public:
        unsigned int getPos() const 
        { 
            return pos; 
        }
        Insert(unsigned int pos) 
        : pos(pos) 
        { 
            *this << std::fixed;
        }
    };
    
    std::vector<Block*> gcode_blocks; // the actual buffer (should always contain at least one Block to write to)
    
    std::list<Insert> inserts; // comands to be inserted in the gcode when writing it to file/commandSocket
    
    double total_time; // the total amount of time it takes to print all gcode currently in this buffer
    
    std::ostream* output_stream;
public:
    GcodeBuffer()
    : total_time(0.0)
    , output_stream(&std::cout)
    { 
        gcode_blocks.push_back(new Block());
    }
    
    GcodeBuffer& operator*() 
    {
        return *this;
    }
    
    void setOutputStream(std::ostream* stream)
    {
        output_stream = stream;
        *output_stream << std::fixed;
    }
    
    template<typename T>
    GcodeBuffer& operator<<(T t) 
    {
        *gcode_blocks.back() << t;
        return *this;
    }
    
    void estimate(double time_estimate)
    {
        gcode_blocks.back()->time = time_estimate;
        total_time += time_estimate;
        gcode_blocks.push_back(new Block());
    }
    
    /*!
     * Creates a gcode writable at (at least) \p time before the end of the gcode in buffer, or after the beginning of the gcode in this buffer.
     */
    std::stringstream& insert(double time, bool before)
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
    
    /*!
     * Flush the content of the buffer, including the inserts.
     * 
     * Write content to the output_stream and
     * reset all gcode, inserts and time estimates in the buffer.
     */
    void flush()
    {
        inserts.sort([](const Insert& a, const Insert& b) -> bool { return a.pos < b.pos; } );
        for (unsigned int block_idx = 0; block_idx < gcode_blocks.size(); block_idx++)
        {
            while ( ! inserts.empty() && block_idx == inserts.front().getPos())
            { // insert all inserts which should be inserted at this position (in arbitrary order)
                *output_stream << inserts.front().str();
                inserts.pop_front();
            }
                
            Block& block = *gcode_blocks[block_idx];
            *output_stream << block.str();
        }
        while ( ! inserts.empty())
        { // insert all inserts which should be inserted at the end (and all inserts which were accidentally skipped)
            *output_stream << inserts.front().str();
            inserts.pop_front();
        }
        
        for (Block* block : gcode_blocks)
        {
            delete block;
        }
        gcode_blocks.clear();
        gcode_blocks.push_back(new Block());
        inserts.clear();
        total_time = 0.0;
    }
};


} // namespace cura 



#endif
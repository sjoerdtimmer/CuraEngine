/** Copyright (C) 2015 Ultimaker - Released under terms of the AGPLv3 License */
#ifndef GCODE_BUFFER_H
#define GCODE_BUFFER_H

#include <sstream>
#include <vector>
#include <ostream>
#include <algorithm>
#include <list>
#include <iostream>
#include <cassert>

#include "utils/logoutput.h"
#include "timeEstimate.h"

namespace cura 
{

/*!
 * Block of gcode commands with the estimated time it takes to execute them.
 * 
 * A block is supposed to be a single command with the time it takes to execute it, but since some commands do not take time 
 * the block should consist of a couple of commands which are executed immmediately, 
 * followed by a single command which takes time (retraction or move).
 */
class GcodeBlock : public std::stringstream
{ // must be declared outside of GcodeBuffer because timeEstimate.h needs a forward declaration to it.
public:
    double time; //!< estimation of the time it takes to print the gcode commands present in this block of gcode
    TimeEstimateCalculator::Block time_block; //!< A representation of a machine move for time estimation. Don't calculate time from the time_block if it already != zero
    bool finished;
    GcodeBlock() 
    : time(0.0) 
    , finished(false)
    { 
        *this << std::fixed;
    }
};
    
     
/*!
 * A class used to buffer gcode before writing it.
 * 
 * It keeps track of time estimates, so that commands can later be inserted into the buffer.
 */
class GcodeBuffer 
{
    
    class Insert : public std::stringstream
    {
        unsigned int pos; //!< position of the item in GcodeBuffer::gcode_blocks before which to insert gcode of this Insert
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
    
    std::vector<GcodeBlock*> gcode_blocks; // the actual buffer (should always contain at least one Block to write to)
    
    TimeEstimateCalculator estimateCalculator;
    
    double totalPrintTime; //!< The total print time of the whole print
    
    double total_time; // the total amount of time it takes to print all gcode currently in this buffer
    
    std::list<Insert> inserts; // comands to be inserted in the gcode when writing it to file/commandSocket
    
    std::ostream* output_stream;
public:
    GcodeBuffer()
    : estimateCalculator(gcode_blocks)
    , total_time(0.0)
    , output_stream(&std::cout)
    { 
        gcode_blocks.push_back(new GcodeBlock());
    }
    
    
    double getTotalPrintTime()
    {
        return totalPrintTime;
    }

    void resetTotalPrintTime()
    {
        totalPrintTime = 0;
        assert(gcode_blocks.size() == 1); // this should never be called. The gcode_blocks should already be empty due to the last flush of the previous slicing! (The buffer always contains one block)
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
    
    /*!
     * Conclude a block of code by making it a single unit receiving a time estimate
     */
    void estimate(double time);
    
    /*!
     * Conclude a block of code by making it a single unit receiving a time estimate
     */
    void estimate(TimeEstimateCalculator::Position newPos, double feedrate);
    
    /*!
     * Creates a gcode writable at (at least) \p time before the end of the gcode in buffer, or after the beginning of the gcode in this buffer.
     * 
     * \param time The time before/after which to insert gcode
     * \param before Whether to insert the gcode before the end of the current buffer or after the start of the current buffer.
     * \return An insert to which gcode can be written.
     */
    std::stringstream& insert(double time, bool before);
    
    /*!
     * Flush the content of the buffer, including the inserts.
     * 
     * Write content to the output_stream and
     * reset all gcode, inserts and time estimates in the buffer.
     */
    void flush();
};


} // namespace cura 



#endif
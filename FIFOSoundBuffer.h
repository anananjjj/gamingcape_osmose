/*****************************************************************************
*
* File: FIFOSoundBuffer.h
*
* Project: OldSpark emulator.
*
* Description: This class define Circular sound buffer utility.
*
* Author: Vedder Bruno
* Date: 12/07/2006, 20h00 Chile, 'Detroit de Magellan'
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef FIFO_SOUND_BUFFER_H
#define FIFO_SOUND_BUFFER_H
#include "BasicTypes.h"

class FIFOSoundBuffer
{
public:
    /* Constructor. */
    FIFOSoundBuffer(u32 size);

    /* Destructor. */
    ~FIFOSoundBuffer() { delete data_; }
    
    /* Write a value if buffer is not full. */
    bool write(s16 value);
    
    /* read a single value if available. */
    inline s16 read();

    /* Read multiple values. */
    void read(s16 *, u32 nbr);
    
    /* Return true if at least one sample is available. */
    bool dataAvailable() { return (total_generated_ > total_played_); }
    
    /* return true is there is space for more sample. */
    bool spaceAvailable() {return (total_generated_ - total_played_ < data_size_); }
    
    u32 numberDataAvailable() { return total_generated_ - total_played_; }
    
    /* Reset the FIFO. */
    void reset();
    
private:
    u32 data_size_;  /* Size of the circular buffer. */
    s16 *data_; /* Pointer to our buffer. */
    u32 generated_index_;
    u32 played_index_;
    u32 total_generated_;
    u32 total_played_;  
};

#endif

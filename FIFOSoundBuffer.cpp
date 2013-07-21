/*****************************************************************************
*
* File: FIFOSoundBuffer.cpp
*
* Project: OldSpark emulator.
*
* Description: This class implements Circular sound buffer utility.
*
* Author: Vedder Bruno
* Date: 12/07/2006, 20h00 Chile, 'Detroit de Magellan'
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "FIFOSoundBuffer.h"
#include <stdio.h>

FIFOSoundBuffer::FIFOSoundBuffer(u32 size)
{
    data_ = new s16 [size];
    data_size_ = size;
    generated_index_ = 0;
    played_index_ = 0;
    total_generated_ = 0;
    total_played_ = 0;
}

bool FIFOSoundBuffer::write(s16 value)
{
    if ( spaceAvailable() )
    {
        /* We have free space in our buffer. */
        data_[ generated_index_++ ] = value;
        total_generated_++;
        
        /* Do we reach the end of the buffer ?*/
        if (generated_index_ == data_size_)
        {
            generated_index_ = 0;
        }
        return true;
    }
    else
    {    
        //printf("BF\n");
        /* The buffer is full, cancel write operation. */
        return false;
    }
}

/* read a single value if available.    */
/* If no values are available return 0. */
s16 FIFOSoundBuffer::read()
{    
    if (dataAvailable())
    {
        s16 ret = data_[played_index_++];
        total_played_++;
        
        if (played_index_ == data_size_)
        {
            played_index_ = 0;
        }
        return ret; 
    }
    else
    {
        //printf("BE\n");
        return 0;
    }
}

/* Read multiple values.  If there are not enough value available in the buffer */
/* Data will be filled with 00.                                                 */
void FIFOSoundBuffer::read(s16 *dst, u32 nbr)
{
    /* We have enough sample, copy them ! */
    for (u32 i = 0; i < nbr; i++)
    {
        dst[i] = read();
    }
//    printf(" Rem: %d\n",numberDataAvailable());
}

/* Read multiple values. */
void FIFOSoundBuffer::reset()
{
    generated_index_ = 0;
    played_index_ = 0;
    total_generated_ = 0;
    total_played_ = 0; 
}

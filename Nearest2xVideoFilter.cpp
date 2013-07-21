/*****************************************************************************
*
* File: Nearest2xVideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Nearest Neighbour 2x Video Filter.
*
* Author: Vedder Bruno
* Date 17/08/06 18:11 Tupiza, Bolivia.
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#include "VideoFilter.h"
#include "Nearest2xVideoFilter.h"

Nearest2xVideoFilter::Nearest2xVideoFilter()
{
    w_ = 256;
    h_ = 192;
}

int Nearest2xVideoFilter::getFinalOutputWidth()
{
    return w_ * 2;
}

int Nearest2xVideoFilter::getFinalOutputHeight()
{
    return h_ * 2;
}

void Nearest2xVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    u16 *src;
    u32 *dst;
    u32 src_index = 0;
    u32 dst_index = 0;
    src = (unsigned short *) s->pixels;
    dst = (unsigned int *) d->pixels;   ; 
  
    for (u32 o = 0; o < h_; o++)
    {    
        for (u32 i = 0; i < w_; i++)
        {
            u16 col = src[src_index++];
            u32 pix = (col | col << 16);
            dst[dst_index] = pix;
            dst[dst_index + w_] = pix;
            dst_index++;
        }
        dst_index += w_;
    }
}

string Nearest2xVideoFilter::getFilterName()
{
    return string("Nearest 2x Video Filter - Bruno Vedder.");
}

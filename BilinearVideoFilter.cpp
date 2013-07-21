/*****************************************************************************
*
* File: BilinearVideoFilter.cpp
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Bilinear Video Filter.
*
* Author: Vedder Bruno
* Date: 03/04/2005, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "BilinearVideoFilter.h"

BilinearVideoFilter::BilinearVideoFilter()
{
    w_ = 256;
    h_ = 192;
}

int BilinearVideoFilter::getFinalOutputWidth()
{
    return w_*2;
}

int BilinearVideoFilter::getFinalOutputHeight()
{
    return h_*2;
}

unsigned short BilinearVideoFilter::getFilteredPixel(u16 c1, u16 c2)
{
         /* 0xF7DE = 1111011111011110. We use it as mask to zero RGB bit0.  */
         /* Then we can get filtered value without extracting RGB channels. */
         c1 &= 0xF7DE;
         c2 &= 0xF7DE;
         return (c1 + c2)>>1;
}

void BilinearVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    unsigned short *src, *dst, *s_src, *s_dst;

    src = (unsigned short *) s->pixels;
    dst = (unsigned short *) d->pixels;    
    s_src = src;
    s_dst = dst;

    //
    //    Mangle source picture:
    //	
    //	
    //	ABC     AxBxC
    //	DEF =>  xxxxx
    //          DxExF
    //  Where x is the point to be interpolated.
    //	
    //
    for (u32 o = 0; o < h_; o++)
    {
		for (u32 i = 0; i < w_; i++)
		{
		    *dst =  *src;
		    dst++;
		    dst++;
	    	src++;
		}
	dst += (w_<< 1);
    }

    // Linear interpolation.
    dst = s_dst;
    dst++;
    for (u32 o=0; o < h_; o++)
    {
    	for (u32 i = 1; i < w_; i++)
    	{
            *dst = getFilteredPixel(*(dst-1), *(dst+1));
    	    dst++;
    	    dst++;
    	}
    	dst += (w_<< 1)+2;
    }

    // Linear interpolation.
    u32 inc_1 = w_ << 1;
    dst = s_dst;
    dst += inc_1;

    for (u32 o=0; o < h_ -1; o++)
    {
    	for (u32 i = 0; i < inc_1; i++)
    	{
            *dst = getFilteredPixel(*(dst-inc_1), *(dst+inc_1));
    	    dst++;
    	}
	    dst += inc_1;
    }
}

string BilinearVideoFilter::getFilterName()
{
    return string("Bilinear Video Filter.");
}



/*****************************************************************************
*
* File: TvVideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Tv Video Filter.
*
* Author: Vedder Bruno
* Date: 02/02/2005, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "VideoFilter.h"
#include "TvVideoFilter.h"

#define LUMINOSITY .90f

// Constructor.
TvVideoFilter::TvVideoFilter()
{
    tvPrecalc = new unsigned short[65536];
    
    // Precalculate * tv attenuation values.
    for (unsigned int i=0; i < 65536; i++)
    {
        unsigned char r,g,b;
	    unsigned short c;
	
        r = (unsigned char)(float(i >> 11) * LUMINOSITY);
        g = (unsigned char)(float((i >> 5) & 0x3f) * LUMINOSITY);
        b = (unsigned char)(float(i & 0x1F) * LUMINOSITY);        
	    c = (r<<11) | (g<<5) | b;
	    tvPrecalc[i] = c;
    }
}

int TvVideoFilter::getFinalOutputWidth()
{
    return 512;
}

int TvVideoFilter::getFinalOutputHeight()
{
    return 384;
}

void TvVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    unsigned short *src, *dst;
    unsigned short c1;
    unsigned int col,acol;

    src = (unsigned short *) s->pixels;
    dst = (unsigned short *) d->pixels;    
    unsigned int *c_dst = (unsigned int *)dst;
    unsigned int *c_src = (unsigned int *)src;
    
    for (int o = 0; o < 192; o++)
    {
	for (int i = 0; i < 128; i++)
	{
	    acol = *c_src;				// 32bits access.

	    c1 = (unsigned short)(acol &0xFFFF);	// 1st pixel.
	    col = (c1 <<16) | c1;			// double pixel
	    *c_dst = col;				// Write it !
	    c1 = tvPrecalc[c1];				// Get attenuated value.
	    col = (c1 <<16) | c1;			// double pixel.
	    c_dst[256] = col;				// Write it.
            c_dst++;
	    
	    c1 = (unsigned short)(acol>>16);		// Second pixel.
	    col = (c1 <<16) | c1;
	    *c_dst = col;
	    c1 = tvPrecalc[c1];
	    col = (c1 <<16) | c1;
	    c_dst[256] = col;
            c_dst++;
	    
	    c_src++;
	}
        c_dst+=256;					// Next line 256*4 = 512*2 :-)
    }
}


string TvVideoFilter::getFilterName()
{
    return string("Tv Video Filter");
}


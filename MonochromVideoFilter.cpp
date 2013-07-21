/*****************************************************************************
*
* File: MonochromVideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Monochrom Video Filter.
* It's done for testing the video interface. It's not usefull filter.
*
* Author: Vedder Bruno
* Date: 02/02/2005, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "VideoFilter.h"
#include "MonochromVideoFilter.h"

int MonochromVideoFilter::getFinalOutputWidth()
{
    return 256;
}

int MonochromVideoFilter::getFinalOutputHeight()
{
    return 192;
}

void MonochromVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    
    unsigned short *src = (unsigned short *) s->pixels;
    unsigned short *dst = (unsigned short *) d->pixels;    
    unsigned char r,g,b;
    unsigned short col;
    
    for (int o = 0; o < 192; o++)
    {
	for (int i = 0; i < 256; i++)
	{
	    col = *src;
	    r = (col >> 11); 
	    g = (col >> 5) & 0x3F;
	    b = (col & 0x1F);
            b = (unsigned char) ((r + (g>>1) + b) / 3);
	    col = ( (b <<11) | (b << 6) | b);
	    *dst = col;
	    dst++;
	    src++;
	}
    }	
}

string MonochromVideoFilter::getFilterName()
{
    return string("Monochrom Video Filter");
}

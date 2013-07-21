/*****************************************************************************
*
* File: Scale2xVideoFilter.cpp
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Scale2x Video Filter.
*
* This implementation is based on algorithm given by Andrea Mazzoleni.
*
* Web site: http://scale2x.sourceforge.net/
*
* Author: Vedder Bruno
* Date: 02/02/2005, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "Scale2xVideoFilter.h"

int Scale2xVideoFilter::getFinalOutputWidth()
{
    return 512;
}

int Scale2xVideoFilter::getFinalOutputHeight()
{
    return 384;
}

#define PA src[ind - 256 - 1]
#define PB src[ind - 256 ]
#define PC src[ind - 256 + 1]

#define PD src[ind - 1]
#define PE src[ind]
#define PF src[ind + 1]

#define PG src[ind + 256 - 1]
#define PH src[ind + 256 ]
#define PI src[ind + 256 + 1]

#define E0 *dst
#define E1 dst[1]
#define E2 dst[512]
#define E3 dst[513]


void Scale2xVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    unsigned short *src, *dst;
    int ind;

    src = (unsigned short *) s->pixels;
    dst = (unsigned short *) d->pixels;    
    FilterTopBorder(src, dst);
    
    ind = 257;
    dst+= 1024;
    dst++;
    dst++;
    for (int o = 1; o < 191; o++)
    {
	for (int i = 1; i < 255; i++)
	{
	    if ( (PB != PH) && (PD !=PF) )
	    {
                E0 = (PD == PB) ? PD : PE;
                E1 = (PB == PF) ? PF : PE;
                E2 = (PD == PH) ? PD : PE;
                E3 = (PH == PF) ? PF : PE;		
	    }
	    else
	    {
	        E0 = PE;
	        E1 = PE;
	        E2 = PE;
	        E3 = PE;
	    }
	    dst++;
	    dst++;
    	    ind++;
	}
        ind += 2;
	dst +=512+4;
    }
    FilterBottomBorder(src, dst);
}

string Scale2xVideoFilter::getFilterName()
{
    return string("Scale2x Video Filter (algorithm from A.Mazzoleni).");
}

void Scale2xVideoFilter::FilterTopBorder(unsigned short *src,unsigned short *dst)
{
    int ind = 1;
 
    dst++;
    dst++;
         
    for (int i = 1; i < 255; i++)
    {
        if (PD !=PF && PE !=PH)
	{
            E0 = PE;
	    E1 = PE;
	    E2 = (PD == PH) ? PD : PE;
	    E3 = (PH == PF) ? PH : PE;
	}
	else
	{
	    E0 = PE;
	    E1 = PE;
	    E2 = PE;
	    E3 = PE;
	}
        dst++;
	dst++;
	ind++;
    }
}

void Scale2xVideoFilter::FilterBottomBorder(unsigned short *src,unsigned short *dst)
{
    int ind = 256*191+1;
         
    for (int i = 1; i < 255; i++)
    {
        if (PD !=PF && PE !=PB)
	{
            E0 = (PD == PB) ? PD : PE;
	    E1 = (PB == PF) ? PB : PE;
	    E2 = PE;
	    E3 = PE;
	}
        else
	{
	    E0 = PE;
	    E1 = PE;
	    E2 = PE;
	    E3 = PE;
	}
        dst++;
	dst++;
	ind++;
    }
}

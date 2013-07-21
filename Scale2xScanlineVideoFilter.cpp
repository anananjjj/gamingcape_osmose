/*****************************************************************************
*
* File: Scale2xScanlineVideoFilter.cpp
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Scale2x + Scanline 75% Video Filter.
*
* This implementation is based on algorithm given by Andrea Mazzoleni.
*
* Web site: http://scale2x.sourceforge.net/
*
* Author: Vedder Bruno
* Date: 19/09/2006, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#include "Scale2xScanlineVideoFilter.h"
#define LUMINOSITY .75f

Scale2xScanlineVideoFilter::Scale2xScanlineVideoFilter()
{
    w_ = 256;
    h_ = 192;
    tvPrecalc = new u16[65536];
    
    // Precalculate * tv attenuation values.
    for (u32 i=0; i < 65536; i++)
    {
        u8 r,g,b;
        u16 c;
	
        r = (u8)(float(i >> 11) * LUMINOSITY);
        g = (u8)(float((i >> 5) & 0x3f) * LUMINOSITY);
        b = (u8)(float(i & 0x1F) * LUMINOSITY);        
	    c = (r<<11) | (g<<5) | b;
	    tvPrecalc[i] = c;
    }
}

s32 Scale2xScanlineVideoFilter::getFinalOutputWidth()
{
    return w_ * 2;
}

s32 Scale2xScanlineVideoFilter::getFinalOutputHeight()
{
    return h_ * 2;
}

#define PA src[ind - w_ - 1]
#define PB src[ind - w_ ]
#define PC src[ind - w_ + 1]

#define PD src[ind - 1]
#define PE src[ind]
#define PF src[ind + 1]

#define PG src[ind + w_ - 1]
#define PH src[ind + w_ ]
#define PI src[ind + w_ + 1]

#define E0 *dst
#define E1 dst[1]
#define E2 dst[w_ * 2]
#define E3 dst[w_ * 2 + 1]


void Scale2xScanlineVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    u16 *src, *dst;
    s32 ind;

    src = (unsigned short *) s->pixels;
    dst = (unsigned short *) d->pixels;    
    FilterTopBorder(src, dst);
     	           
    ind = w_ + 1;
    dst+= w_ * 4;
    dst++;
    dst++;
    for (u32 o = 1; o < h_ - 1; o++)
    {
	for (u32 i = 1; i < w_ - 1; i++)
	{
	    if ( (PB != PH) && (PD !=PF) )
	    {
                E0 = (PD == PB) ? PD : PE;
                E1 = (PB == PF) ? PF : PE;
                E2 = (PD == PH) ? tvPrecalc[PD] : tvPrecalc[PE];
                E3 = (PH == PF) ? tvPrecalc[PF] : tvPrecalc[PE];		
	    }
	    else
	    {
	        E0 = PE;
	        E1 = PE;
	        E2 = tvPrecalc[PE];
	        E3 = tvPrecalc[PE];
	    }
	    dst++;
	    dst++;
    	    ind++;
	}
        ind += 2;
	dst +=w_ * 2 +4;
    }
    FilterBottomBorder(src, dst);
}

string Scale2xScanlineVideoFilter::getFilterName()
{
    return string("Scale2x + Scanline Video Filter (algorithm from A.Mazzoleni - Vedder Bruno).");
}

void Scale2xScanlineVideoFilter::FilterTopBorder(u16 *src,u16 *dst)
{
    s32 ind = 1;
 
    dst++;
    dst++;
         
    for (u32 i = 1; i < w_ -1; i++)
    {
        if (PD !=PF && PE !=PH)
	{
       E0 = PE;
	    E1 = PE;
	    E2 = (PD == PH) ? tvPrecalc[PD] : tvPrecalc[PE];
	    E3 = (PH == PF) ? tvPrecalc[PH] : tvPrecalc[PE];
	}
	else
	{
	    E0 = PE;
	    E1 = PE;
        E2 = tvPrecalc[PE];
        E3 = tvPrecalc[PE];
	}
        dst++;
	dst++;
	ind++;
    }
}

void Scale2xScanlineVideoFilter::FilterBottomBorder(u16 *src,u16 *dst)
{
    s32 ind = w_*(h_ - 1) + 1;
         
    for (u32 i = 1; i < w_ -1; i++)
    {
        if (PD !=PF && PE !=PB)
	{
        E0 = (PD == PB) ? PD : PE;
	    E1 = (PB == PF) ? PB : PE;
	    E2 = tvPrecalc[PE];
        E3 = tvPrecalc[PE];
	}
        else
	{
	    E0 = PE;
	    E1 = PE;
	    E2 = tvPrecalc[PE];
        E3 = tvPrecalc[PE];
	}
        dst++;
	dst++;
	ind++;
    }
}

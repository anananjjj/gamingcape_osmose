/*****************************************************************************
*
* File: Scale2xScanlineVideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Scale2x + Scanline Video Filter.
*
* This implementation is based on algorithm given by Andrea Mazzoleni.
*
* Web site: http://scale2x.sourceforge.net/
*
* Author: Vedder Bruno
* Date: 01/11/2006, 14h40
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#ifndef SCALE2XSCANLINE_H
#define SCALE2XSCANLINE_H
#include "BasicTypes.h"
#include "VideoFilter.h"

class Scale2xScanlineVideoFilter : public VideoFilter {
public:    
    Scale2xScanlineVideoFilter();
    s32 getFinalOutputWidth();
    s32 getFinalOutputHeight();
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();

private:
    void FilterTopBorder(u16 *s,u16 *d);
    void FilterBottomBorder(u16 *s,u16 *d);
    void FilterLeftBorder(u16 *s,u16 *d);
    void FilterRighttBorder(u16 *s,u16 *d);
    u16 *tvPrecalc;
    u32 w_;
    u32 h_;
};

#endif

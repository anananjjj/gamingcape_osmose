/*****************************************************************************
*
* File: BilinearVideoFilter.h
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
#ifndef BILINEAR_H
#define BILINEAR_H

#include "VideoFilter.h"
#include "BasicTypes.h"

class BilinearVideoFilter : public VideoFilter {
public:    
    BilinearVideoFilter();
    int getFinalOutputWidth();
    int getFinalOutputHeight();
    unsigned short getFilteredPixel(u16 c1, u16 c2);
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();
protected:
    unsigned int h_;
    unsigned int w_;
};

#endif

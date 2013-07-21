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

#ifndef NEAREST_2X_VIDEO_FILTER_H
#define NEAREST_2X_VIDEO_FILTER_H
#include "BasicTypes.h"
#include "VideoFilter.h"

class Nearest2xVideoFilter : public VideoFilter
{

public:    
           
    Nearest2xVideoFilter();
    s32 getFinalOutputWidth();
    s32 getFinalOutputHeight();
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();

private:
    u32 w_;
    u32 h_;    
};

#endif

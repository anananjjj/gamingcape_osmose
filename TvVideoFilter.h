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
#ifndef TV_VIDEO_FILTER_H
#define TV_VIDEO_FILTER_H

#include "VideoFilter.h"
#include "IOMapper.h"

class TvVideoFilter : public VideoFilter {
public:
    TvVideoFilter();
    int getFinalOutputWidth();
    int getFinalOutputHeight();
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();

private:
    unsigned short *tvPrecalc;
};

#endif

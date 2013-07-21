/*****************************************************************************
*
* File: NullVideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a code for the Null Video Filter.
*
* Author: Vedder Bruno
* Date: 02/02/2005, 16h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "VideoFilter.h"
#include "NullVideoFilter.h"

int NullVideoFilter::getFinalOutputWidth()
{
    return 256;
}

int NullVideoFilter::getFinalOutputHeight()
{
    return 192;
}

void NullVideoFilter::Filter(SDL_Surface *s, SDL_Surface *d)
{
    SDL_BlitSurface(s, NULL, d, NULL);
}

string NullVideoFilter::getFilterName()
{
    return string("Null Video Filter");
}

/*****************************************************************************
*
* File: MonochromVideoFilter.h
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

class MonochromVideoFilter : public VideoFilter {
public:    

    int getFinalOutputWidth();
    int getFinalOutputHeight();
    void Filter(SDL_Surface *s, SDL_Surface *d);
    string getFilterName();

};

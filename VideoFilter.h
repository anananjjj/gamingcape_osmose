/*****************************************************************************
*
* File: VideoFilter.h
*
* Project: Osmose emulator.
*
* Description: This class provide a software interface for video filters.
*
* getFinalOutputWidth() should return final screen width allocated by SDL
*    in pixels.
*
* getFinalOutputHeight() should return final screen height allocated by SDL
*    in pixels.
*
* Filter should perform the filter computation. s is normal screen generated
*    by osmose (typically 256*192 16bit 565 RGB buffer), d is the displayed
*    buffer, of size provided by getFinalOutputWidth()/getFinalOutputHeight()
*    still in 16bits 565 RGB format.
*
* GetFilterName should return a string containing the filter name.
*
* To create new video Filter:
* Write a class which derive this one, and provide appropriate code for
* Virtual methods. Then add the new .h declaration file into VideoFilterList.h
* and add a #define corresponding to your filter type.
* Modify parseCommandLine in main.cpp to catch a new command line option.
* Finaly, instanciate the right Object in OsmoseCore's constructor appropriate
* switch section. Add .o in Makefile
*
* Author: Vedder Bruno
* Date: 02/02/2005, 14h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef VIDEO_FILTER_H
#define VIDEO_FILTER_H

#include <SDL/SDL.h>
#include <iostream>

using namespace std;

class VideoFilter
{
public:
    virtual ~VideoFilter(){};
    virtual int getFinalOutputWidth() = 0;
    virtual int getFinalOutputHeight() = 0;
    virtual void Filter(SDL_Surface *s, SDL_Surface *d) = 0;
    virtual string getFilterName() = 0;
};

#endif

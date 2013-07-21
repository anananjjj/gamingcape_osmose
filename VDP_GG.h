/*****************************************************************************
*
* File: VDP_GG.h
*
* Project: Osmose emulator.
*
* Description: This class will handle GAMEGEAR's VDP (Video Display Processor)operation.
*
* Author: Vedder Bruno
* Date: 25/10/2006, 19h15
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef VDP_GG_H
#define VDP_GG_H

#include "VDP.h"

class VDP_GG : public VDP
{
public:
    VDP_GG(Z80 *c, bool ntsc);
    void writeDataPort(unsigned char data);
    void traceBackGroundLine(SDL_Surface *s);
};

#endif

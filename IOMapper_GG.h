/*****************************************************************************
*
* File: IOMapper_GG.h
*
* Project: Osmose emulator.
*
* Description: This class will handle GAMEGEAR emulator input/output operation.
*
* Author: Vedder Bruno
* Date: 08/10/2004, 18h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef IO_MAPPER_GG_H
#define IO_MAPPER_GG_H

#include "IOMapper.h"

using namespace std;

class IOMapper_GG : public IOMapper
{
public:

    IOMapper_GG(VDP &, SN76489 &);		// Constructor.
    virtual ~IOMapper_GG(){};
    void reset();
    unsigned char in8(unsigned  address);
    void out8(unsigned  address, unsigned  char data);

private:

};
#endif

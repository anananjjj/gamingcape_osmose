/*****************************************************************************
*
* File: SaveState.h
*
* Project: Osmose emulator.
*
* Description: This class provide an interface for Savestate.
*
* Author: Vedder Bruno
* Date: 27.11.2006 10:31:16 
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef SAVESTATE_H
#define SAVESTATE_H

#include <fstream>
#include <iostream>
#include <iomanip>

#include "BasicTypes.h"

using namespace std;

class ImplementsSaveState
{
public:
    virtual bool saveState(ofstream &ofs) = 0;
    virtual bool loadState(ifstream &ifs) = 0;
    virtual ~ImplementsSaveState() {};
};

#endif

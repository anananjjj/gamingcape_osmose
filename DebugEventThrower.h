/*****************************************************************************
*
* File: DebugEventThrower.h
*
* Project: Osmose emulator.
*
* Description: This interface declares behaviour for a thrower of debug events.
* It's done to help debugging SMS/GG hardware and Osmose code.
* 
* Author: Vedder Bruno
*
* Date: 16.11.2006 10:42:28 
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef DEBUG_EVENT_THROWER_H
#define DEBUG_EVENT_THROWER_H

#include <iostream>
#include <iomanip>

#include "DebugEventListener.h"

using namespace std;

class DebugEventThrower
{
public:

    DebugEventThrower() { del = NULL;}
    void attachDebugEventListener(DebugEventListener *el);
	void throwDebugEvent(int type, char *source, char *msg);

private:

	DebugEventListener *del;
};

#endif


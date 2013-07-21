/*****************************************************************************
*
* File: DebugEventThrower.cpp
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
#include "DebugEventThrower.h"

void DebugEventThrower::attachDebugEventListener(DebugEventListener *el)
{
    del = el;
}

void DebugEventThrower::throwDebugEvent(int type,char *source, char *msg)
{
    if (del != NULL)
	{
		del->sendDebugEvent(type, source, msg);
    }
	else
	{
	    cerr << "Trying to throw DebugEvent but the Thrower is not attached to the listener !" << endl;
	}
}







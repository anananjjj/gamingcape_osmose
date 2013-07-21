/*****************************************************************************
*
* File: DebugEventListener.h
*
* Project: Osmose emulator.
*
* Description: This interface declares behaviour for Listening debug events.
* It's done to help debugging SMS/GG hardware and Osmose code.
* How it's used:
* SmsDebugger or a logger for example implements the DebugEventListener
* interface. The VDP, or any debugEventThrower will be attach to the 
* DebugEventListener. To throw debug event the thrower will use throwDebugEvent
* method. 
* Author: Vedder Bruno
*
* Date: 16.11.2006 10:42:28 
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef DEBUG_EVENT_LISTENER_H
#define DEBUG_EVENT_LISTENER_H

/* Debug Event types. */
enum
{
	DbgEvtUserTrigger 			= 0,
	DbgEvtCpuStep	  			= 1,
	DbgEvtScanlineBreakpoint	= 2,
	DbgEvtCpuIrqAsserted		= 3,
	DbgEvtCpuDisableInterrupt   = 4,
	DbgEvtCpuEnableInterrupt   =  5,
	DbgEvtGeneric				= 256
};

class DebugEventListener
{
public:

    virtual void sendDebugEvent(int event_type, char *sourcec, char *message) = 0;
	virtual ~DebugEventListener(){};
};

#endif


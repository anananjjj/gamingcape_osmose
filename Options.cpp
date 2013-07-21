/*****************************************************************************
*
* File: Options.cpp
*
* Project: Osmose emulator.
*
* Description: This file contains resets function for Options, and 
* emulator options. It's nothing else than default values.
*
* Author: Vedder Bruno
* Date: 11/01/2005, 18h40
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "Options.h"
#include "MemoryMapper.h"

void Options::reset()
{
    // Setup defaut machine options.
    WorldVersion = true;		            // Not japanese sms, Export machine.
    MachineType = SMS;                     	// Default Machine Type.
    inputType = DEFAULT_PAD;	            // Default PAD instead SPORT PAD.
    acceleration = DEFAULT_ACCELERATION;	// Default Paddle acceleration.
    ntsc = true;		                	// Default NTSC video timing.
    mapperType = SegaMapper;		        // No codemaster mapper.
	irq_hack = false;
}

void EmulatorOptions::reset()
{
    // Setup defaut emulator options.
    fullscreen_flag   = false;
    videoFilterType   = 0;
    sound             = true;
    bright_palette    = true;
    default_config    = true;
    display_fps       = false;
    joy1              = 0;
    joy2              = 1;
    joystart          = 2;
    joypause          = 3;
    memset(ini_file,0,512);
}

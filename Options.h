/*****************************************************************************
*
* File: Options.h
*
* Project: Osmose emulator.
*
* Description: This class is just a global variable receptor.
*
* Author: Vedder Bruno
* Date: 11/01/2005, 18h40
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include "Bits.h"

#define JAPAN	0
#define EXPORT	1

#define DEFAULT_ACCELERATION 0.5

enum
{
	DEFAULT_PAD = 0,
	PADDLE      = 1,
 	JOYSTICK	= 2
};

enum
{
    SMS,
    GAMEGEAR
};

using namespace std;

class Options
{
public:

    bool WorldVersion;	// 0 is Jap, 1 Export machine.
    int  MachineType;   // SMS or GameGear ?
    int inputType;		// Input device type.
    float acceleration; // Paddle acceleration.
    int mapperType;		// Use Sega/Codemaster/Korean Mapper.	
    bool ntsc;			// true = ntsc false = pal/secam.
	bool irq_hack;
    void reset();
};

class EmulatorOptions
{
public:

    bool bright_palette;	// Bright palette or not ?
    bool fullscreen_flag;	// Emu must run in fullscreen.
    bool sound;			// Emu must emulate SN76489.
    bool default_config;	// Does the emulator use default conf ?
    bool display_fps;	        // FPS in Title bar option.
    int videoFilterType;        // Emu must apply tv filter.
    char ini_file[512];         // For name of file containing conf.

    int joy1;
    int joy2;
    int joystart;
    int joypause;

    void reset();
};

#endif

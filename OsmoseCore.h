/*****************************************************************************
*
* File: OsmoseCore.h
*
* Project: Osmose emulator.
*
* Description: This file contains Osmose main loop, handling keyboard, SDL
* event, and hardware emulation.
*
* Author: Vedder Bruno
* Date: 23/01/2005, 14h13
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef OSMOSE_CORE_H
#define OSMOSE_CORE_H

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_audio.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "./cpu/Z80.h"
#include "SmsEnvironment.h"
#include "VDP.h"
#include "VDP_GG.h"
#include "SN76489.h"
#include "Options.h"
#include "SmsDebugger.h"
#include "./z80_disasm/Z80Dasm.h"
#include "Bits.h"
#include "WaveWriter.h"
#include "OsmoseConfiguration.h"
#include "TextWriter.h"
#include "VideoFilter.h"
#include "VideoFilterList.h"
#include "Version.h"
#include "InputDevice.h"
#include "PadInputDevice.h"
#include "PaddleInputDevice.h"
#include "JoystickInputDevice.h"
#include "Definitions.h"
#include "DebugEventThrower.h"
#include "SaveState.h"

using namespace std;

/* Use save state slot from 1 to 5. */
enum
{
	minSaveStateSlot = 1,
	maxSaveStateSlot = 5
};

class OsmoseCore : public DebugEventThrower
{

public:

    OsmoseCore(const char *rom_f);	// Constructor.
    void run();				// Enter Osmose main loop.
    void run_frame();

private:

    OsmoseConfiguration *oc;	     // OsmoseConfig.
    SDL_AudioSpec        format;     // Format structure to get our 22050hz 16bits sound.
    SDL_Surface         *screen;     // Surface for rendered filtered image 256x192 or 512x384.
    SDL_Surface         *buffer;     // SMS screen, without modification 256x192.
    string               game_name;  // Used as prefix for screenshot, sound shot and save states.
    VDP                 *v;	     // Video Display Processor object.
    MemoryMapper        *mem;        // Memory mapper Object.
    SmsEnvironment      *env;        // Z80 Cpu environment.
    IOMapper            *iom;        // Input/Output mapper Object.
    InputDevice         *input;	     // Pad or Paddle or trackball.
    Z80                 *cpu;	     // Alessandro Scotti Z80 core.
    WaveWriter          *wavW;       // Sound ripper.
    VideoFilter         *vf;         // VideoFilter.
    TextWriter          *tw;         // Text Writer.
    SDL_Event           event;       // SDL event used for keyboard handling.
    SDL_TimerID         t_id;        // SDL Timer used for 60hz synchro.
    float               gain;        // gain level.

#ifdef BUILT_IN_DEBUGGER
    unsigned int   old_cycles;	     // Used for cycle count.
    Z80Dasm        *dasm;	     // Z80 disasembler object.
    SmsDebugger    *dbg;	     // Sms Debugger.
    bool           exec_f;           // Continue cpu exec Flag.
#endif

    // Note on frame_skip_increment and frame_skip counter:
    // In order to have correct emulation speed, Osmose implements
    // an auto-frame-skip feature. The Algorithm is quite simple:
    // Each time a frame should be rendered, Osmose sums frame_skip_counter
    // and frame_skip increment. If frame_skip_counter is >=0 a frame is
    // rendered. Else, aframe is skipped. This means that for:
    // frame_skip_increment = 1.0 every frames are rendered (60fps).
    // frame_skip_increment = 0.5 half frames are rendered (30fps).
    // frame_skip_increment = 0.8 implies 48fps frame rate.
    // Values >1.0 are sensless, and implies 60fps.

    float frame_skip_increment;		// frame skip limit
    float frame_skip_counter;		// frame skip counter.


    bool nmi;				// nmi flag, used with keyboard pause.
    bool quit;				// quit flag, keyboard esc is pressed.
    bool sound_shot_toggle;		// Flag, for start/stop recording sound.
    int  screenshotNbr;			// Screenshot number appended to save.
    int  tileshotNbr;			// Tile rip  number appended to save.
    int  soundshotNbr;			// Sound rip  number appended to save.
    int  saveStateSlotNbr;		// Which slot will be use for save state.
    const char *rom_filename;		// Pointer to argv[1].

    void save_bbr();
    void handleSDLKeyboardEvent(SDL_Event e);
    void captureScreen();
    void captureTiles(VDP *vd);
    void displayTiles(SDL_Surface *s, VDP *vd, int tile, int x, int y);
    void setupAudioFormat();
    void setupSDLVideo(VideoFilter *v);
    void setupSDLAudio();
    bool saveSaveState();
    bool loadSaveState();
};

#endif

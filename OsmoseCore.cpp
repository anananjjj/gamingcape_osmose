/*****************************************************************************
*
* File: OsmoseCore.cpp
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
#include "OsmoseCore.h"

SDL_sem           *timer;      // Semaphore used for FPS synchronisation.
SN76489           *p;

// These methods are not from OsmoseCore
unsigned int timer_callback(unsigned int i, void *p);
void sndCallback(void *ud, unsigned char *s, int len);

/*--------------------------------------------------------------------*/
/* This method is the OsmoseCore constructor.			      */
/*--------------------------------------------------------------------*/
OsmoseCore::OsmoseCore(const char *rom_f)
{
    quit           = false;
    nmi		   = false;
    sound_shot_toggle = false;
    frame_skip_increment = .70;
    frame_skip_counter =0.0;
    screenshotNbr  = 0;
    tileshotNbr	   = 0;
    soundshotNbr   = 0;
    saveStateSlotNbr = minSaveStateSlot;
    rom_filename   = rom_f;
    gain = 0.00f;

    // Setup default parameters.
    if (emu_opt.default_config == true)
    {
        oc = new OsmoseConfiguration();
    }
    else
    {
        oc = new OsmoseConfiguration(emu_opt.ini_file);
    }

    /* Instanciate TextWriter. */
    tw = new TextWriter();

    /* Note: After instantiation, opt.MachineType contains SMS or GAMEGEAR type. */
    mem  = new MemoryMapper(rom_filename, oc->user_home_folder.c_str());
    if (opt.mapperType == CodemasterMapper) mem->setMapperType(CodemasterMapper);
    if (opt.mapperType == KoreanMapper) mem->setMapperType(KoreanMapper);

	env  = new SmsEnvironment();
    cpu  = new Z80(*env);

    p    = new SN76489(3579545, 44100);

    if (opt.MachineType == SMS)
    {
        v    = new VDP(cpu, opt.ntsc);   // Instanciate ntsc or pal SMS VDP.
        iom  = new IOMapper(*v, *p);
    }
    else
    {
        // Instanciate ntsc (always) GAMEGEAR VDP. Force emulator option too.
        v = new VDP_GG(cpu, true);
        opt.ntsc = true;
        iom  = new IOMapper_GG(*v, *p);
    }

    env->setIOMapper(iom);
    env->setMemoryMapper(mem);
    env->setVDP(v);
    env->setCPU(cpu);
    wavW = NULL;
    game_name = mem->getROMName();
    vf = new NullVideoFilter();

#ifdef BUILT_IN_DEBUGGER
    dbg  = new SmsDebugger();
	v->attachDebugEventListener(dbg);
    this->attachDebugEventListener(dbg);
	cpu->attachDebugEventListener(dbg);
	mem->attachDebugEventListener(dbg);
	iom->attachDebugEventListener(dbg);
    dasm = new Z80Dasm(*env);
    dbg->setMemoryMapper(mem);
    dbg->setEnvironment(env);
    dbg->setVDP(v);
    dbg->setIOMapper(iom);
    dbg->setDasm(dasm);
    dbg->setCPU(cpu);
    old_cycles = 0;
#endif

    // Instanciate the right VideoFilter.
    switch (emu_opt.videoFilterType)
    {
        case NULL_FILTER:
            vf = new NullVideoFilter();
		break;

        case TV_FILTER:
            vf = new TvVideoFilter();
		break;

        case MONOCHROM_FILTER:
            vf = new MonochromVideoFilter();
		break;

        case SCALE2X_FILTER:
            vf = new Scale2xVideoFilter();
		break;

        case BILINEAR_FILTER:
            vf = new BilinearVideoFilter();
		break;

    	case SCALE2XSCANLINE_FILTER:
            vf = new Scale2xScanlineVideoFilter();
		break;

    	case NN2X_FILTER:
            vf = new Nearest2xVideoFilter();
		break;
        default:
            vf = new NullVideoFilter();
		break;
    }

    setupSDLVideo(vf);

	/* Instanciation of input device is done here beacause JOYSTICK needs to be */
	/* initialised with SDL_init first.											*/
	switch (opt.inputType)
    {
        case DEFAULT_PAD:
            input= new PadInputDevice(iom, oc);
		break;

        case PADDLE:
	    	input= new PaddleInputDevice(iom, oc);
		break;

		case JOYSTICK:
			input= new JoystickInputDevice(iom, oc);
		break;

		default:
            input= new PadInputDevice(iom, oc);
		break;
    }

    cout << "Using as sms input device: " << input->getInputDeviceName() << endl;

    if (emu_opt.sound == true)
    {
        setupSDLAudio();
    }

    timer = SDL_CreateSemaphore(0);
    t_id = SDL_AddTimer(DELAY_BETWEEN_FRAME, timer_callback, NULL);
}

/*--------------------------------------------------------------------*/
/* This method handles SDL keyboard events.			      */
/*--------------------------------------------------------------------*/
void OsmoseCore::handleSDLKeyboardEvent(SDL_Event e)
{
    ostringstream msg;
    int k = e.key.keysym.sym;

    if(e.type == SDL_KEYDOWN)
    {
        if ( k == oc->PAUSE_KEY)
		{
            nmi = true;
			return;
        }

        if ( k == oc->TILESHOT_KEY)
		{
            captureTiles(v);
			return;
        }

        if ( k == oc->SCREENSHOT_KEY)
		{
            captureScreen();
			return;
		}

		/* Save State key is not configurable. */
        if ( k == SDLK_F12)
		{
            bool succes = false;
			succes = saveSaveState ();
			msg << "State saved in slot:" << saveStateSlotNbr;
			if (succes) tw->addText(msg.str().c_str(), 180);
			else tw->addText ("Save state failed!", 180);
			return;
		}

		/* Load State key is not configurable. */
        if ( k == SDLK_F11)
		{
            bool succes = false;
			succes = loadSaveState ();
    		msg << "State loaded from slot:" << saveStateSlotNbr;
			if (succes) tw->addText(msg.str().c_str(), 180);
			else tw->addText("Load state failed!", 180);
			return;
		}

		/* Decrement Save state slot key is not configurable. */
        if ( k == SDLK_KP_MINUS)
		{
			if (saveStateSlotNbr > minSaveStateSlot)
			{
				saveStateSlotNbr -=1;
			}
			msg << "using save state slot:" << saveStateSlotNbr;
			tw->addText(msg.str().c_str(), 180);
			return;
		}

		/* Increment Save state slot key is not configurable. */
        if ( k == SDLK_KP_PLUS)
		{
			if (saveStateSlotNbr < maxSaveStateSlot)
			{
				saveStateSlotNbr +=1;
			}
			msg << "using save state slot:" << saveStateSlotNbr;
			tw->addText(msg.str().c_str(), 180);
			return;
		}
#ifdef BUILT_IN_DEBUGGER
        if ( k == oc->DEBUGGER_KEY)
		{
            throwDebugEvent(DbgEvtUserTrigger, "OsmoseCore", "User trigger.");
	    	return;
        }
#endif
    }

	/* Key released event.*/
    if(e.type == SDL_KEYUP)
    {
        if ( k == oc->QUIT_KEY)
		{
            quit = true;
	    	return;
        }

		if (k == oc->SOUNDSHOT_KEY)
		{
            if (emu_opt.sound == true)
	    	{
        		if (sound_shot_toggle == true)
				{
	            	// We were recording sound, so stop it now.
                    tw->addText("stopping sound recording.", 120);
       	   	    	wavW->close();
		    		delete wavW; // To avoid memory leak.
		    		sound_shot_toggle = false;
				}
				else
				{
        	    	char snd_shot_filename[256];
	            	// We weren't recording sound, record it now, so create WaveWriter.
#ifdef __USE_UNIX98
                    sprintf(snd_shot_filename,"./osmose_files/snd/%s(%d).wav",game_name.c_str(), soundshotNbr);
#else
                    sprintf(snd_shot_filename,".\\snd\\%s(%d).wav", game_name.c_str(), soundshotNbr);
#endif
		   			wavW = new WaveWriter( snd_shot_filename );
		    		sound_shot_toggle = true;
		    		soundshotNbr++;
                    tw->addText("starting sound recording!", 120);
				}
	    	}
	    	return;
		}
    }

    // We had handled general emulation key, now call specific sms handler.
    input->handleDeviceChange(e);
}

/*--------------------------------------------------------------------*/
/* This method runs one frame.					      */
/* Note about frame variable:					      */
/* This variable is the total number of frame (displayed or not !)    */
/* emulated by Osmose. This value is use for speed synchronisation at */
/* 60 Fps. That's why the value is incremented even if the frame isn't*/
/* rendered.                                                          */
/*                                                                    */
/* rPi: This was taken from osmose-0.9.0beta.                         */
/*--------------------------------------------------------------------*/
void OsmoseCore::run_frame()
{
    bool drawline    = true;
    bool snd_started = false;
    bool played      = false;
    float snd_update = 0;
    int scanline_number;
    unsigned int over_cycles = 0;

    scanline_number = (opt.ntsc == true) ? 262 : 313; // NTSC or PAL

	if (nmi == true)
	{
		cpu->nmi();
		nmi = false;
	}

	for (v->line=0; v->line < scanline_number; v->line++)
	{
		over_cycles = cpu->run(CPU_CYCLES_PER_LINE - over_cycles);
		if (emu_opt.sound == true)
		{
			snd_update+=(float)SND_TOGGLE;  // Needed to call update_sound_buffer 367.5/frame
			played = p->run(2);
			if (sound_shot_toggle == true && played==true)
			{
				wavW->writeData(p->getLastSample());
			}

			if (snd_update > 1.0f)
			{
				played = p->run(2);
				if (played == true)
				{
					snd_update = snd_update - (float)1.0f;
				}
				if (sound_shot_toggle == true && played==true)
				{
					wavW->writeData(p->getLastSample());
				}
			}
		}
		v->update(buffer, drawline);
	} // For i = 0 to scanline_number

	tw->update(buffer, drawline);


        if (drawline == true)
	{
            // Apply Filter, then update screen.
            vf->Filter(buffer, screen);
            SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	if (snd_started == false && emu_opt.sound == true)
	{
		// Start playing only if sound buffer is full.
		// This avoid playing silence on emulator startup.
		if (!p->getFIFOSoundBuffer()->spaceAvailable())
		{
			snd_started = true;
			SDL_PauseAudio(0); // start playing !
		}
	}

	cpu->setCycles(0);
}

/*--------------------------------------------------------------------*/
/* This method is the main emulation loop.			      */
/* Note about frame variable:					      */
/* This variable is the total number of frame (displayed or not !)    */
/* emulated by Osmose. This value is use for speed synchronisation at */
/* 60 Fps. That's why the value is incremented even if the frame isn't*/
/* rendered.                                                          */
/*--------------------------------------------------------------------*/
void OsmoseCore::run()
{
    int frame = 0;

    // tw->addText(__OSMOSE_VERSION__,120);

    cout << "Starting emulation." << endl;

    while(!quit)
    {
	if (frame % 3 == 0)
		SDL_SemWait(timer);

	run_frame();
	frame++;

        /* Handle SDL Events */
        while( SDL_PollEvent( &event ) )
        {
			if (event.type == SDL_QUIT) quit = true;
            handleSDLKeyboardEvent(event);
        }


		if (nmi == true)
        {
            cpu->nmi();
            nmi = false;
        }

/* #ifdef AUTO_SCREENSHOT
		if (frame == 30*60) captureScreen();
#endif */
        // Trackball, Paddle need to be updated in time.
    	input->updateDevice();

        // To avoid overflow of cycles_ that cause cpu to halt.
		//cout << "OverCycles per frame : " << dec << (cpu->getCycles() - 59736) << endl;
        cpu->setCycles(0);
    } // While (!quit)

    SDL_CloseAudio();
#ifdef OPCODES_STATS
    cpu->opcodes_stats();
#endif
    save_bbr();
/*
#ifndef BUILT_IN_DEBUGGER
            cout << "Leaving emulation..." << endl;
	        cout << "Total frames    :" << dec << frame << endl;
            cout << "Rendered frames :" << dec << (frame - skipped_frame) << endl;
            cout << "Skipped frames  :" << dec << skipped_frame<< endl;
            cout << "Emulation time  :" << (float)(SDL_GetTicks()/1000.0) << " sec." << endl;
            cout << "Aver. frame rate:" << (float)((frame -  skipped_frame)/(float)(SDL_GetTicks()/1000)) << " fps." << endl;
#endif
*/
    // We may be recording sound when leaving emulation. If it's the case, close sound_shot file.
    if (sound_shot_toggle == true && emu_opt.sound == true)
    {
        wavW->close();
    }
    SDL_Quit();
}

/*--------------------------------------------------------------------*/
/* This method will save as bitmap, vdp graphics tiles.		      */
/* First, a 128x224 SDL_Surface is allocated.			      */
/* Then tiles are drawn there.					      */
/* A screenshot is taken					      */
/* The Surface is freed.					      */
/* Filename is tiles_rip_ + game_name.bmp.			      */
/*--------------------------------------------------------------------*/
void OsmoseCore::captureTiles(VDP *v)
{
    int status;
    char sName[256];
    unsigned short map_p = 0;
    SDL_Surface *tiles;

    // Allocate new software surface.
    tiles = SDL_CreateRGBSurface(SDL_SWSURFACE, 128,224,16,0xF800,0x7E0,0x1f,0x0);
    if (tiles == NULL)
    {
        cerr << "Couldn't get 128x224 surface: %s" << endl << SDL_GetError() << endl;;
        cerr << "Tiles are not saved." << endl << SDL_GetError() << endl;;
    }

    // Draw tiles there.
    for (int o=0; o<28;o++)
	for (int i=0; i<16;i++)
	{
	    int tile = map_p;
	    displayTiles(tiles, v, tile, i<<3, o<<3);
	    map_p++;
	}
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    // Save it !
#ifdef __USE_UNIX98
    sprintf(sName,"./osmose_files/tiles/gfx_%s(%d).bmp", game_name.c_str(), tileshotNbr);
#else
    sprintf(sName,".\\tiles\\gfx_%s(%d).bmp", game_name.c_str(), tileshotNbr);
#endif

    tileshotNbr++;
    SDL_LockSurface(tiles);
    status = SDL_SaveBMP(tiles, sName);
    SDL_UnlockSurface(screen);
    if(status == 0)
    {
       tw->addText("gfx have been saved.", 120);
    }
    else
    {
       tw->addText("fail to save gfx!", 120);

    }
    SDL_FreeSurface(tiles);
}


/*--------------------------------------------------------------------*/
/* This method draws a tile n, at position x,y, assuming that the     */
/* Surface is 128 pixels wide.					      */
/*--------------------------------------------------------------------*/
void OsmoseCore::displayTiles(SDL_Surface *s, VDP *vd, int tile, int x, int y)
{
    unsigned short *ptr;
    unsigned char col_index, p0, p1, p2, p3;
    unsigned int ti, c;

    ti = tile<<5;
    ptr = (unsigned short *)s->pixels + ((y<<7)+x );
    for(int o=0; o<8;o++)
    {
	c = (o<<2) + ti;
	p0 = vd->VRAM[c++];
	p1 = vd->VRAM[c++];
	p2 = vd->VRAM[c++];
    	p3 = vd->VRAM[c++];

	for (int i=0; i<8;i++)
	{
	    col_index = (p0 >>7) | (p1 >> 7)<<1 | (p2 >> 7)<<2 | (p3 >> 7)<<3;
	    *(unsigned short *)ptr = vd->colors[col_index];
	    ptr++;
	    p0<<=1;
	    p1<<=1;
	    p2<<=1;
	    p3<<=1;
	}
    ptr += 120; // Complement to next line, based on 256 pixel width.
    }
}

/*--------------------------------------------------------------------*/
/* This method is used to synchronise emulator at good FPS. Each      */
/* DELAY_BETWEEN_FRAME milliseconds, the semaphore is freed. Then, the*/
/* main loop can continue it's execution. Note that DELAY_.. is 50ms. */
/* This is done to avoid problems due to timer granularity. Timer<20ms*/
/* aren't very accurate. So instead of drawing frame, waiting 16ms for*/
/* three times, we draw 3 frames, and wait 50ms. 50ms is large enough */
/* to get good synchronisation.					      */
/*--------------------------------------------------------------------*/
unsigned int timer_callback(unsigned int i, void *p)
{
    SDL_SemPost(timer);
    return DELAY_BETWEEN_FRAME;
}

/*--------------------------------------------------------------------*/
/* This method setup our default sound.				      */
/*--------------------------------------------------------------------*/
void OsmoseCore::setupAudioFormat()
{
    format.freq     = 44100;
    format.format   = AUDIO_S16LSB;
    format.channels = 1;
    format.samples  = SAMPLE_SIZE;
    format.callback = sndCallback;
    format.userdata = NULL;
}

/*--------------------------------------------------------------------*/
/* This method is called by SDL sound system, to fill the sound buffer*/
/* s is the place to put sound data, len is length of buffer in bytes.*/
/*--------------------------------------------------------------------*/
void sndCallback(void *ud, unsigned char *s, int len)
{
	p->getWave(s, len);
}

/*--------------------------------------------------------------------*/
/* This method setup SDL video system.				      */
/*--------------------------------------------------------------------*/
void OsmoseCore::setupSDLVideo(VideoFilter *v)
{
    /* Initialize SDL */
    if ( SDL_Init(SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0 ) {
            cerr << "Couldn't initialize SDL: "<< endl << SDL_GetError() << endl;
            exit(1);
    }

    cout << "Active VideoFilter is: " << v->getFilterName() << endl;
    SDL_WM_SetCaption(__OSMOSE_VERSION__,__OSMOSE_VERSION__);    /* Window title, Iconified widows title */

    int w = v->getFinalOutputWidth();
    int h = v->getFinalOutputHeight();

    SDL_ShowCursor(SDL_DISABLE);
    if (emu_opt.fullscreen_flag == false)
    {
        if ( (screen=SDL_SetVideoMode(w, h,16, SDL_SWSURFACE)) == NULL ) {
                cerr << "Couldn't set video mode: %s" << endl << SDL_GetError() << endl;;
                exit(2);
        }
    }
    else
    {
        if ( (screen=SDL_SetVideoMode(w, h,16, SDL_FULLSCREEN)) == NULL ) {
                cerr << "Couldn't set video mode: %s" << endl << SDL_GetError() << endl;;
                exit(2);
        }
        SDL_ShowCursor(SDL_DISABLE);
    }

    // Allocate our 256*192 16bits buffer.
    buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 256,192,16,0xF800,0x7E0,0x1f,0x0);
    if (buffer == NULL)
    {
        cerr << "Couldn't get 256x192x16 surface: %s" << endl << SDL_GetError() << endl;;
        exit (1);
    }
    cout << "Video surfaces successfully allocated." << endl;
}

/*--------------------------------------------------------------------*/
/* This method setup SDL audio system.				      */
/*--------------------------------------------------------------------*/
void OsmoseCore::setupSDLAudio()
{
    setupAudioFormat();
    int r = SDL_OpenAudio(&format, NULL);

    if (r >= 0)
    {
        cout << "Audio device successfully opened." << endl;
    }
    else
    {
        cerr << "Couldn't open audio device:" << endl << SDL_GetError() ;
        cerr << "Activating -nosound option." << endl;
        cerr << "Disabling  -snd_shot option." << endl;
        emu_opt.sound    = false;
    }
}

/*--------------------------------------------------------------------*/
/* This method takes a screenshot of the game. The filename is        */
/* game_name +x .bmp , where x is the number of taken screenshot,     */
/* which is incremented every time captureScreen() is called.         */
/*--------------------------------------------------------------------*/
void OsmoseCore::captureScreen()
{
    int status;
    char sName[256];
#ifdef __USE_UNIX98
    sprintf(sName,"./osmose_files/screen/%s(%d).bmp", game_name.c_str(), screenshotNbr);
#else
    sprintf(sName,".\\screen\\%s(%d).bmp", game_name.c_str(), screenshotNbr);
#endif
    screenshotNbr++;
    SDL_LockSurface(screen);
    status = SDL_SaveBMP(screen, sName);
    SDL_UnlockSurface(screen);
    if(status == 0)
    {
       tw->addText("screenshot saved.", 120);
    }
    else
    {
       tw->addText("fail to save screenshot!", 120);
    }
}

/*--------------------------------------------------------------------*/
/* This method generates save state file. Here is the format:         */
/* File Offset - Data type.                                           */
/* 0000-0003     unsigned char[4] marker "OESS"                       */
/* 0004-0007     unsigned char[4] 0 + version that create savestate.  */
/* 0008-0019     unsigned char[18] z80_8bits_registers.               */
/* 001A-0021     unsigned short[4] z80_16bits_registers.              */
/*--------------------------------------------------------------------*/
bool OsmoseCore::saveSaveState()
{
	ostringstream save_state_name;
#ifdef __USE_UNIX98
    save_state_name << "./osmose_files/saves/" << mem->getROMName() << "_slot_" << saveStateSlotNbr <<".sta";
#else
    	save_state_name << ".\\osmose_files\\saves\\" << mem->getROMName() << "_slot_" << saveStateSlotNbr <<".sta";
#endif


    ofstream output_file((char *)save_state_name.str().c_str(), ios::out | ios::binary );
    if (output_file.is_open() == false )
    {
	    return false;
    }

    /* Write cpu data. */
    if (!cpu->saveState( output_file ) ) {cout << "CPU save fail." << endl; return false;}

    /* Write memory mapper data. */
    if (!mem->saveState( output_file ) ) {cout << "Mem Mapper save fail." << endl; return false;}

    /* Write VDP data. */
    if (!v->saveState( output_file ) ) {cout << "VDP save fail." << endl; return false;}

    /* Write SN76489 data. */
    if (!p->saveState( output_file ) ) {cout << "PSG save fail." << endl; return false;}
    output_file.close();
    return true;
}

bool OsmoseCore::loadSaveState()
{
	ostringstream load_state_name;
#ifdef __USE_UNIX98
    load_state_name << "./osmose_files/saves/" << mem->getROMName() << "_slot_" << saveStateSlotNbr <<".sta";
#else
    load_state_name << ".\\osmose_files\\saves\\" << mem->getROMName() << "_slot_" << saveStateSlotNbr <<".sta";
#endif

    ifstream input_file((char *)load_state_name.str().c_str(), ios::in | ios::binary);
    if (input_file.is_open() == false )
    {
	    return false;
    }

    /* Load cpu data. */
    if (!cpu->loadState( input_file ) ) {cout << "CPU load fail." << endl; return false;}

    /* Load memory mapper data. */
	if (!mem->loadState( input_file ) ) {cout << "Mem Mapper load fail." << endl; return false;}

	/* Load VDP data. */
    if (!v->loadState( input_file ) ) {cout << "VDP load fail." << endl; return false;}

    /* Save SN76489 data. */
    if (!p->loadState( input_file ) ) {cout << "PSG load fail." << endl; return false;}
    input_file.close();
    return true;
}

/*--------------------------------------------------------------------*/
/* This method saves Battery Backed Memory if needed.                 */
/*--------------------------------------------------------------------*/
void OsmoseCore::save_bbr()
{
    char full_name[256];

#ifdef __USE_UNIX98
    sprintf(full_name,"./osmose_files/bbr/%s.bbr", game_name.c_str());
#else
    sprintf(full_name,".\\bbr\\%s.bbr", game_name.c_str());
#endif
    mem->save_battery_backed_memory( string(full_name) );
}


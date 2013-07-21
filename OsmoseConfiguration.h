/*****************************************************************************
*
* File: OsmoseConfiguration.h
*
* Project: Osmose emulator.
*
* Description: This class will handle emulator all things related to Osmose
* configuration, like keyboard config, or default path ect...
*
* Author: Vedder Bruno
* Date: 08/10/2004, 18h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef OSMOSE_CONFIGURATION_H
#define OSMOSE_CONFIGURATION_H

#include <SDL/SDL.h>
#include <iostream>
#include <iomanip>
#include <fstream>

//#define CONFIGURATION_VERBOSE

using namespace std;

typedef struct SdlKeyEntry
{
    string key_name;
    int  key_value;
};

extern SdlKeyEntry key_list[];


class OsmoseConfiguration
{
public:

    OsmoseConfiguration(const char *ini_file);		// Constructor.
    OsmoseConfiguration();				// Constructor.

    int P1_UP_KEY;
    int P1_DOWN_KEY;
    int P1_LEFT_KEY;
    int P1_RIGHT_KEY;
    int P1_A_KEY;
    int P1_B_KEY;
    int P2_UP_KEY;
    int P2_DOWN_KEY;
    int P2_LEFT_KEY;
    int P2_RIGHT_KEY;
    int P2_A_KEY;
    int P2_B_KEY;
    int GG_START_KEY;
    int SCREENSHOT_KEY;
    int TILESHOT_KEY;
    int SOUNDSHOT_KEY;
    int PAUSE_KEY;
    int QUIT_KEY;
    int DEBUGGER_KEY;
    
    // The folder where Osmose can read/write config, screenshots, soundshots, tileshots, ...
    string user_home_folder;
    
private:

    bool loadConfigurationFromFile(const char *f);
    int  findSDLKeySym(string wrd);
    void analyseLine(string l);
    void reset();
};

#endif

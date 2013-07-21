/*****************************************************************************
*
* File: OsmoseConfiguration.cpp
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
#include "OsmoseConfiguration.h"

#ifdef __USE_UNIX98
#include <pwd.h> // For passwd structure
#include <unistd.h> // For getlogin, ...
#include <sys/stat.h> // For mkdir, chown, ...
#endif
#ifdef __WIN32
#include <windows.h>
#endif

OsmoseConfiguration::OsmoseConfiguration(const char *filename)
{
    bool ret = false;
    string default_filename;

    // Try to load config from file.
    #ifdef __USE_UNIX98 // Unices
    passwd * password;
    char * Temp = getlogin();

    if ( Temp != NULL )
    {
        password = getpwnam(Temp);
    }
    else
    {
        password = getpwuid(geteuid());
    }

    if ( password != NULL)
    {
        user_home_folder = password->pw_dir;
        //User = password->pw_name; // Good to know
        default_filename = user_home_folder + "/.osmose/osmose.ini";
    }
    #endif
    ret = loadConfigurationFromFile(filename);
    if (ret != true)
    {
        // If unsuccessful, setup default configuration.
	reset();
    }
}

OsmoseConfiguration::OsmoseConfiguration()
{
    // Here we should attempt to read default configuration file first,
    // And create one if there aren't.
    
    // The folder where it will we be created, and where it will be read by default depends on the OS we are using.
    // I'll give here the code to be used under Linux/Unix/BSD, and Windows...
    string default_filename;
    #ifdef _WIN32
        default_filename = "osmose.ini";
    #endif
    #ifdef __USE_UNIX98 // Unices
        default_filename = "./osmose_files/osmose.ini";
    #endif
    
    // Try to load config from file.
    bool ret = false;
    ret = loadConfigurationFromFile(default_filename.c_str());
    if (ret != true)
    {
    // If unsuccessful, setup default configuration.
    // And don't forget to create a default config file...
    reset();

    // Default config creation
    #ifdef __USE_UNIX98 // Unices.

    // Create Osmose directory.
	int md = 0;
	md = mkdir( string("./osmose_files").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create ./osmose_files/ directory." << endl;
	}

        // Create bbr subdirectory.
	md = mkdir( string("./osmose_files/bbr").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create bbr subdirectory." << endl;
	}

        // Create snd subdirectory.
	md = mkdir( string("./osmose_files/snd").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create snd subdirectory." << endl;
	}

        // Create screen subdirectory.
	md = mkdir( string("./osmose_files/screen").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create screen subdirectory." << endl;
	}

        // Create tile subdirectory.
	md = mkdir( string("./osmose_files/tiles").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create tiles subdirectory." << endl;
	}

    // Create Save State subdirectory.
	md = mkdir( string("./osmose_files/saves").c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
	if (md == -1)
	{
	    cout << "Unable to create saves subdirectory." << endl;
	}

    #endif // __USE_UNIX98
    #ifdef __WIN32
	bool md = CreateDirectory(".\\screen", NULL);
	if (!md){cout << "Unable to create screen subdirectory." << endl;}
	md = CreateDirectory(".\\tiles", NULL);
	if (!md){cout << "Unable to create tiles subdirectory." << endl;}
	md = CreateDirectory(".\\snd", NULL);
	if (!md){cout << "Unable to create snd subdirectory." << endl;}
	md = CreateDirectory(".\\bbr", NULL);
	if (!md){cout << "Unable to create bbr subdirectory." << endl;}
	md = CreateDirectory(".\\saves", NULL);
	if (!md){cout << "Unable to create saves subdirectory." << endl;}

    #endif

    ofstream file(default_filename.c_str(), ios::out);
    if (file.is_open() == false ) 
    {
        cerr << "Unable to create '" << default_filename << "' init file. Default configuration loaded." << endl;
    }
    else // Default ini file creation
    {
        file << "#########################################################################\n";
        file << "#\n";                                      
        file << "# This is an example of Osmose .ini file.\n";
        file << "# It can be used to modify keyboard configuration.\n";
        file << "# Lines beginning with '#' are considered as comment and\n";
        file << "# will be ignored.\n";
        file << "#\n";
        file << "# For retrieving key correspondance osmose looks for these\n";
        file << "# tags:\n";
        file << "#\n";
        file << "#     SCREENSHOT\n";
        file << "#     SOUNDSHOT\n";
        file << "#     QUIT\n";
        file << "#     TILESHOT\n";
        file << "#     DEBUGGER\n";
        file << "#     PAUSE\n";   
        file << "#     PAD1_UP\n";
        file << "#     PAD1_DOWN\n";  
        file << "#     PAD1_LEFT\n";  
        file << "#     PAD1_RIGHT\n";  
        file << "#     PAD1_BUTTON_A\n";
        file << "#     PAD1_BUTTON_B\n";
        file << "#     PAD2_UP\n";
        file << "#     PAD2_DOWN\n";  
        file << "#     PAD2_LEFT\n";
        file << "#     PAD2_RIGHT\n";  
        file << "#     PAD2_BUTTON_A\n";
        file << "#     PAD2_BUTTON_B\n";
        file << "#     GAMEGEAR_START\n";
        file << "#\n";
        file << "# If a tag is found, it looks for a valid key definition.\n";
        file << "# The list of valid keys are listed in comment at the end of\n";
        file << "# this file. All definition begins w   ith'SDLK'.n \n";
        file << "# Note that missing tags will result in disabled key. You can\n";
        file << "# disallow screenshot in emulation by commenting the line with\n";
        file << "# SCREENSHOT tag.\n";
        file << "#\n";
        file << "#########################################################################\n\n";

        file << "# Configuration Starts Here !\n\n";

        file << "#\n";
        file << "#  General emulation keys:\n";
        file << "#\n\n";

        file << "SCREENSHOT    = SDLK_F2\n";
        file << "SOUNDSHOT     = SDLK_F1\n";
        file << "QUIT          = SDLK_ESCAPE\n";
        file << "TILESHOT      = SDLK_F3\n";
	
        file << "DEBUGGER      = SDLK_d\n";
        file << "PAUSE         = SDLK_p\n\n";

        file << "#\n";
        file << "# First Player PAD:\n";
        file << "#\n\n";

        file << "PAD1_UP       = SDLK_UP\n";
        file << "PAD1_DOWN     = SDLK_DOWN\n";
        file << "PAD1_LEFT     = SDLK_LEFT\n";
        file << "PAD1_RIGHT    = SDLK_RIGHT\n";
        file << "PAD1_BUTTON_A = SDLK_LCTRL\n";
        file << "PAD1_BUTTON_B = SDLK_LALT\n\n";

        file << "#\n";
        file << "# Second Player PAD:\n";
        file << "#\n\n";

        file << "PAD2_UP       = SDLK_KP5\n";
        file << "PAD2_DOWN     = SDLK_KP2\n";
        file << "PAD2_LEFT     = SDLK_KP1\n";
        file << "PAD2_RIGHT    = SDLK_KP3\n";
        file << "PAD2_BUTTON_A = SDLK_n\n";
        file << "PAD2_BUTTON_B = SDLK_b\n\n";

        file << "#\n";
        file << "# GAME GEAR Specific:\n";
        file << "#\n\n";
        file << "GAMEGEAR_START = SDLK_RETURN\n\n";

        file << "# Configuration Ends Here !\n\n\n\n\n\n\n";

        file << "#########################################################################\n";
        file << "#\n";
        file << "#     You can remove this list, it's just provided to help user to\n";
        file << "#     configure osmose emulator.\n";
        file << "#\n";
        file << "#     Valid Key definitions:\n";
        file << "#     ----------------------\n";
        file << "#\n";
        file << "#     SDLK_BACKSPACE\n";
        file << "#     SDLK_TAB\n";
        file << "#     SDLK_CLEAR\n";
        file << "#     SDLK_RETURN\n";
        file << "#     SDLK_PAUSE\n";
        file << "#     SDLK_ESCAPE\n";
        file << "#     SDLK_SPACE\n";
        file << "#     SDLK_EXCLAIM\n";
        file << "#     SDLK_QUOTEDBL\n";
        file << "#     SDLK_HASH\n";
        file << "#     SDLK_DOLLAR\n";
        file << "#     SDLK_AMPERSAND\n";
        file << "#     SDLK_QUOTE\n";
        file << "#     SDLK_LEFTPAREN\n";
        file << "#     SDLK_RIGHTPAREN\n";
        file << "#     SDLK_ASTERISK\n";
        file << "#     SDLK_PLUS\n";
        file << "#     SDLK_COMMA\n";	
        file << "#     SDLK_MINUS\n";
        file << "#     SDLK_PERIOD\n";
        file << "#     SDLK_SLASH\n";
        file << "#     SDLK_0\n";
        file << "#     SDLK_1\n";
        file << "#     SDLK_2\n";
        file << "#     SDLK_3\n";
        file << "#     SDLK_4\n";
        file << "#     SDLK_5\n";
        file << "#     SDLK_6\n";
        file << "#     SDLK_7\n";
        file << "#     SDLK_8\n";
        file << "#     SDLK_9\n";
        file << "#     SDLK_COLON\n";
        file << "#     SDLK_SEMICOLON\n";
        file << "#     SDLK_LESS\n";
        file << "#     SDLK_EQUALS\n";
        file << "#     SDLK_GREATER\n";
        file << "#     SDLK_QUESTION\n";
        file << "#     SDLK_AT\n";
        file << "#\n";
        file << "#     -Don't use uppercase letters\n";
        file << "#\n";
        file << "#     SDLK_LEFTBRACKET\n";
        file << "#     SDLK_BACKSLASH\n";
        file << "#     SDLK_RIGHTBRACKET\n";
        file << "#     SDLK_CARET\n";
        file << "#     SDLK_UNDERSCORE\n";
        file << "#     SDLK_BACKQUOTE\n";
        file << "#     SDLK_a\n";
        file << "#     SDLK_b\n";
        file << "#     SDLK_c\n";
        file << "#     SDLK_d\n";
        file << "#     SDLK_e\n";
        file << "#     SDLK_f\n";
        file << "#     SDLK_g\n";
        file << "#     SDLK_h\n";
        file << "#     SDLK_i\n";
        file << "#     SDLK_j\n";
        file << "#     SDLK_k\n";
        file << "#     SDLK_l\n";
        file << "#     SDLK_m\n";
        file << "#     SDLK_n\n";
        file << "#     SDLK_o\n";
        file << "#     SDLK_p\n";
        file << "#     SDLK_q\n";
        file << "#     SDLK_r\n";
        file << "#     SDLK_s\n";
        file << "#     SDLK_t\n";
        file << "#     SDLK_u\n";
        file << "#     SDLK_v\n";
        file << "#     SDLK_w\n";
        file << "#     SDLK_x\n";
        file << "#     SDLK_y\n";
        file << "#     SDLK_z\n";
        file << "#     SDLK_DELETE\n";
        file << "#\n\n";

        file << "#     -Numeric keypad\n";
        file << "#\n";
        file << "#     SDLK_KP0\n";
        file << "#     SDLK_KP1\n";
        file << "#     SDLK_KP2\n";
        file << "#     SDLK_KP3\n";
        file << "#     SDLK_KP4\n";
        file << "#     SDLK_KP5\n";
        file << "#     SDLK_KP6\n";
        file << "#     SDLK_KP7\n";
        file << "#     SDLK_KP8\n";
        file << "#     SDLK_KP9\n";
        file << "#     SDLK_KP_PERIOD\n";
        file << "#     SDLK_KP_DIVIDE\n";
        file << "#     SDLK_KP_MULTIPLY\n";
        file << "#     SDLK_KP_MINUS\n";
        file << "#     SDLK_KP_PLUS\n";
        file << "#     SDLK_KP_ENTER\n";
        file << "#     SDLK_KP_EQUALS\n";
        file << "#\n";
        file << "#     -Arrows + Home/End p\n";
        file << "#\n";
        file << "#     SDLK_UP\n";
        file << "#     SDLK_DOWN\n";
        file << "#     SDLK_RIGHT\n";
        file << "#     SDLK_LEFT\n";
        file << "#     SDLK_INSERT\n";
        file << "#     SDLK_HOME\n";
        file << "#     SDLK_END\n";
        file << "#     SDLK_PAGEUP\n";
        file << "#     SDLK_PAGEDOWN\n";
        file << "#\n";
        file << "#     -Function keys\n";
        file << "#\n";
        file << "#     SDLK_F1\n";
        file << "#     SDLK_F2\n";
        file << "#     SDLK_F3\n";
        file << "#     SDLK_F4\n";
        file << "#     SDLK_F5\n";
        file << "#     SDLK_F6\n";
        file << "#     SDLK_F7\n";
        file << "#     SDLK_F8\n";
        file << "#     SDLK_F9\n";
        file << "#     SDLK_F10\n";
        file << "#     SDLK_F11\n";
        file << "#     SDLK_F12\n";
        file << "#     SDLK_F13\n";
        file << "#     SDLK_F14\n";
        file << "#     SDLK_F15\n";
        file << "#\n";
        file << "#     -Key state modifier\n";
        file << "#\n";
        file << "#     SDLK_NUMLOCK\n";
        file << "#     SDLK_CAPSLOCK\n";
        file << "#     SDLK_SCROLLOCK\n";
        file << "#     SDLK_RSHIFT\n";
        file << "#     SDLK_LSHIFT\n";
        file << "#     SDLK_RCTRL\n";
        file << "#     SDLK_LCTRL\n";
        file << "#     SDLK_RALT\n";
        file << "#     SDLK_LALT\n";
        file << "#     SDLK_RMETA\n";
        file << "#     SDLK_LMETA\n";
        file << "#     SDLK_LSUPER\n";
        file << "#     SDLK_RSUPER\n";
        file << "#     SDLK_COMPOSE\n";
        file << "#\n";
        file << "#     -Miscellaneous functions\n";
        file << "#\n";
        file << "#     SDLK_HELP\n";
        file << "#     SDLK_PRINT\n";
        file << "#     SDLK_SYSREQ\n";
        file << "#     SDLK_BREAK\n";
        file << "#     SDLK_MENU\n";
        file << "#     SDLK_EURO\n";
        file << "#     SDLK_UNDO\n";
        file << "#\n";
        file << "#########################################################################\n";

	} // End of default ini file creation
	
	file.close();
    }
}

/*--------------------------------------------------------------------*/
/* This method Set up Osmose default configuration.		      */
/*--------------------------------------------------------------------*/
void OsmoseConfiguration::reset()
{
    P1_UP_KEY      = SDLK_UP;
    P1_DOWN_KEY    = SDLK_DOWN;
    P1_LEFT_KEY    = SDLK_LEFT;
    P1_RIGHT_KEY   = SDLK_RIGHT;
    P1_A_KEY       = SDLK_LCTRL;
    P1_B_KEY       = SDLK_LALT;
    P2_UP_KEY      = SDLK_KP5;
    P2_DOWN_KEY    = SDLK_KP2;
    P2_LEFT_KEY    = SDLK_KP1;
    P2_RIGHT_KEY   = SDLK_KP3;
    P2_A_KEY       = SDLK_n;
    P2_B_KEY       = SDLK_b;
    GG_START_KEY   = SDLK_RETURN;
    TILESHOT_KEY   = SDLK_F3;
    SCREENSHOT_KEY = SDLK_F2;
    SOUNDSHOT_KEY  = SDLK_F1;
    PAUSE_KEY      = SDLK_p;
    QUIT_KEY       = SDLK_ESCAPE;
    DEBUGGER_KEY   = SDLK_d;
}

/*--------------------------------------------------------------------*/
/* This method Set up Osmose configuration from by loading it from a  */
/* given file. Lines begining with # are considered to be comment.    */
/* Format expected:						      */
/* PAD1_UP = SDLK_UP						      */
/*--------------------------------------------------------------------*/
bool OsmoseConfiguration::loadConfigurationFromFile(const char *f)
{
    bool ret = false;

    // Reset all keys.
    P1_UP_KEY      = 0;
    P1_DOWN_KEY    = 0;
    P1_LEFT_KEY    = 0;
    P1_RIGHT_KEY   = 0;
    P1_A_KEY       = 0;
    P1_B_KEY       = 0; 
    P2_UP_KEY      = 0; 
    P2_DOWN_KEY    = 0; 
    P2_LEFT_KEY    = 0; 
    P2_RIGHT_KEY   = 0; 
    P2_A_KEY       = 0; 
    P2_B_KEY       = 0; 
    GG_START_KEY   = 0;
    SCREENSHOT_KEY = 0; 
    TILESHOT_KEY   = 0; 
    SOUNDSHOT_KEY  = 0;
    PAUSE_KEY      = 0; 
    QUIT_KEY       = 0; 
    DEBUGGER_KEY   = 0; 

    ifstream file(f, ios::in);
    if (file.is_open() == false ) 
    {
        cerr << "Unable to open '" << f << "' init file. Default configuration loaded." << endl;
        return false;
    }

    ret = true;
        
    while (!file.eof())
    {
        string rd;
	char buffer[256];
	file.getline(buffer, 256);
	rd = string(buffer, 256);
	analyseLine(rd);
    }
    file.close();
    return ret;
}

/*--------------------------------------------------------------------*/
/* This method looks up for an entry into key_list, and return it's   */
/* associated value. If not found, return value is -1.		      */
/*--------------------------------------------------------------------*/
int  OsmoseConfiguration::findSDLKeySym(string wrd)
{
   int ret = -1;
   int i = 0;
   bool found = false;
   
   while (key_list[i].key_value != -1)
   {
       if (key_list[i].key_name == wrd)
       {
           found = true;
	   ret = key_list[i].key_value;
       }
       i++;
   }
   return ret;
}

/*--------------------------------------------------------------------*/
/* This method returns true if the line does not start with # comment */
/* if the line is not empty, and if the line contains a tag, an '='   */
/* and a Valid SDL key name.                                          */
/*--------------------------------------------------------------------*/
void OsmoseConfiguration::analyseLine(string rd)
{
    unsigned int r;
    unsigned int comment_pos;
    string kn;
    int ks;
    
    #define KEY_NBR 19
    
    string k_list[] = {"PAD1_UP","PAD1_DOWN","PAD1_LEFT","PAD1_RIGHT","PAD1_BUTTON_A","PAD1_BUTTON_B",
                       "PAD2_UP","PAD2_DOWN","PAD2_LEFT","PAD2_RIGHT","PAD2_BUTTON_A","PAD2_BUTTON_B",
    		           "PAUSE","QUIT","SCREENSHOT","TILESHOT","DEBUGGER","SOUNDSHOT","GAMEGEAR_START"};    

    for (int i = 0; i < KEY_NBR; i++)
    {
        r = rd.find(k_list[i], 0);
	comment_pos = rd.find("#", 0);
	
	// Comment before TAG, skip this line.
	if ((r !=string::npos) && (comment_pos !=string::npos) && (comment_pos < r))
	   return;
	if (r != string::npos)
	{
	    int ind = 0;
	    unsigned int p;
	    // We have found a known tag.
	    while(key_list[ind].key_value != -1)
	    {
	        p = rd.find(key_list[ind].key_name, 0);
		if (p != string::npos)
		{
#ifdef CONFIGURATION_VERBOSE
                    cout << k_list[i] << " is associated to keysym " << (int)key_list[ind].key_value << endl;
#endif
                    ks = key_list[ind].key_value;
                    switch(i)
		    {
		        case 0:
                            P1_UP_KEY = ks;
			break;
		        case 1:
                            P1_DOWN_KEY = ks;
			break;
		        case 2:
                            P1_LEFT_KEY = ks;
			break;
		        case 3:
                            P1_RIGHT_KEY = ks;
			break;
		        case 4:
                            P1_A_KEY = ks;
			break;
		        case 5:
                            P1_B_KEY = ks;
			break;
		        case 6:
                            P2_UP_KEY = ks;
			break;
		        case 7:
                            P2_DOWN_KEY = ks;
			break;
		        case 8:
                            P2_LEFT_KEY = ks;
			break;
		        case 9:
                            P2_RIGHT_KEY = ks;
			break;
		        case 10:
                            P2_A_KEY = ks;
			break;
		        case 11:
                            P2_B_KEY = ks;
			break;
		        case 12:
                            PAUSE_KEY = ks;
			break;
		        case 13:
                            QUIT_KEY = ks;
			break;
		        case 14:
                            SCREENSHOT_KEY = ks;
			break;
		        case 15:
                            TILESHOT_KEY = ks;
			break;
		        case 16:
                            DEBUGGER_KEY = ks;
			break;
		        case 17:
                            SOUNDSHOT_KEY = ks;
			break;
		        case 18:
                            GG_START_KEY = ks;
			break;

		    }
		}
		ind++;
	    }
	}
    }
}

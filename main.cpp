/*
    Written on 2004 by Bruno Vedder

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*****************************************************************************
*
* File: main.cpp
*
* Project: Osmose emulator.
*
* Author: Vedder Bruno
* Date: 28/10/2004, 13h00
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#include <iostream>
#include <iomanip>
#include "OsmoseCore.h"
#include "Options.h"
#include "Version.h"
#include "VideoFilterList.h"

using namespace std;


// Functions prototypes.
void usage();
int parseCommandLine(int a, char *rv[]);
void splashText();

Options              opt;        // SMS Machine related options.
EmulatorOptions      emu_opt;    // Emulator related options.

/*--------------------------------------------------------------------*/
/* This is the main method of osmose. It check args number, correct   */
/* Emulator options, and start emulation is Ok.                       */
/*--------------------------------------------------------------------*/
int main(int argc,char *argv[])
{
    int ret;

    // Reset default options.
    emu_opt.reset();
    opt.reset();
    
    // Display Osmose version, author.
    splashText();

    // Check arguments number. If not enough display usage.
    if (argc < 2)
    {
        usage();
        exit(0);
    }

    // emu_opt will be corrected depending on cmd line.
    ret = parseCommandLine(argc, argv);

    if (ret) {
	    OsmoseCore core(argv[ret]);
	    core.run();
    } else {
            cout << "No ROM filename has been found on command line (.gg .GG .sms .SMS .zip .ZIP extensions)." << endl;
            usage();
    }

    return 0;    
}

/*--------------------------------------------------------------------*/
/* This method displays osmose usage.				      */
/*--------------------------------------------------------------------*/
void usage()
{
    cout << "Usage is osmose filename, ex: osmose asterix.zip -fs -tv -nosound" << endl << endl;
    cout << "Options: "<< endl;
    cout << "    -paddle           emulates one axis paddle (mapped on mouse)." << endl;
    cout << "    -acceleration x.x paddle acceleration (0.1 to 5 default "<< DEFAULT_ACCELERATION << ")" << endl;
    cout << "    -fs               run in fullscreen   (default: windowed)." << endl;
    cout << "    -nosound          do not play sounds. (default: sound on)."<< endl;
    cout << "    -dp               use dark palette for screen emulation (default: off)." << endl;
    cout << "    -inifile          xxx use xxx as configuration file. " <<endl;
    cout << "    -monochrom        emulates B&W tv (default: off)." << endl;
    cout << "    -scale2x          scale2x video filter implementation (default: off)." << endl;
    cout << "    -bilinear         bilinear video filter (default: off)." << endl;
    cout << "    -tv               emulates TV scanline video filter (default: off)." << endl;
    cout << "    -scale2xscanline  Scale2x + 75% scanline video filter (default: off)." << endl;
    cout << "    -nn2x             nearest neighbour video filter (default: off)." << endl;
    cout << "    -cm               use codemaster games mem. mapper (default: off). " << endl;
    cout << "    -km               use korean games mem. mapper (default: off). " << endl;
    cout << "    -irqhack          Enable irq hack (specific rom option. default off)." << endl;
    cout << "    -pal              emulates PAL/SECAM video timing (default: NTSC)."<< endl;
    cout << "    -jap              run as japanese sms (default: export)." << endl;
    cout << "    -exp              run as exported sms (default)." << endl;
    cout << "    -joy              use joystick as input device, instead of keyboard." << endl;
    cout << "    -joy1 x           define joystick button x as 1" << endl;
    cout << "    -joy2 x           define joystick button x as 2" << endl;
    cout << "    -joystart x       define joystick button x as Start (Game Gear only)" << endl;

}

/*--------------------------------------------------------------------*/
/* This method parses command line, and update options structure with */
/* right options.						      */
/*--------------------------------------------------------------------*/
int parseCommandLine(int a, char *rv[])
{
    int rom_filename_index = 0;

    for (int i=0; i<a; i++)
    {
        if (strcmp(rv[i],"-fs")==0)
        {
            emu_opt.fullscreen_flag = true;
        }

        if (strcmp(rv[i],"-pal")==0)
        {
            opt.ntsc = false;
        }

        if (strcmp(rv[i],"-fps")==0)
        {
            emu_opt.display_fps = true;
        }

        if (strcmp(rv[i],"-jap")==0)
        {
            // Act as Japanese machine.
            opt.WorldVersion = false;  
        }

        if (strcmp(rv[i],"-exp")==0)
        {
            // Act as Exported machine.
            opt.WorldVersion = true;
        }

        if (strcmp(rv[i],"-tv")==0)
        {
            emu_opt.videoFilterType = TV_FILTER;
        }

        if (strcmp(rv[i],"-scale2x")==0)
        {
            emu_opt.videoFilterType = SCALE2X_FILTER;
        }

        if (strcmp(rv[i],"-monochrom")==0)
        {
            emu_opt.videoFilterType = MONOCHROM_FILTER;
        }
        
        if (strcmp(rv[i],"-bilinear")==0)
        {
            emu_opt.videoFilterType = BILINEAR_FILTER;
        }

        if (strcmp(rv[i],"-scale2xscanline")==0)
        {
            emu_opt.videoFilterType = SCALE2XSCANLINE_FILTER;
        }

        if (strcmp(rv[i],"-nn2x")==0)
        {
            emu_opt.videoFilterType = NN2X_FILTER;
        }
        
        if (strcmp(rv[i],"-acceleration")==0)
        {
		    // If actual argument nbr < argument number eg is there any args ?
		    if (i < a-1)
		    {
	            opt.acceleration = atof( rv[i+1]);
				if (opt.acceleration == 0)
				{
			        cerr << "No valid acceleration parameter was given. Using default " << (float)DEFAULT_ACCELERATION << " value." << endl;
		            opt.acceleration = DEFAULT_ACCELERATION;
				}
				cout << "Paddle acceleration used : " << opt.acceleration << endl;
			}
			else
			{
			    cerr << "No acceleration parameter was given. Using default " << (float)DEFAULT_ACCELERATION << " value." << endl;
		        opt.acceleration = DEFAULT_ACCELERATION;
			}
        }

      
        if (strcmp(rv[i],"-nosound")==0)
        {
            emu_opt.sound = false;
        }

        if (strcmp(rv[i],"-cm")==0)
        {
            opt.mapperType = CodemasterMapper;
        }

        if (strcmp(rv[i],"-km")==0)
        {
            opt.mapperType = KoreanMapper;
        }

        if (strcmp(rv[i],"-irqhack")==0)
        {
            opt.irq_hack = true;
        }

        if (strcmp(rv[i],"-paddle")==0)
        {
            opt.inputType = PADDLE;
        }

        if (strcmp(rv[i],"-joy")==0)
        {
            opt.inputType = JOYSTICK;
        }

        if (strcmp(rv[i],"-joy1")==0)
        {
		   if (i < a-1)
	 	   {
	           emu_opt.joy1 = atoi( rv[i+1]);
				if (emu_opt.joy1 < 0)
				{
			        cerr << "No valid button for button 1 was given. Using default value." << endl;
	  	            emu_opt.joy1 = 0;
				}
				cout << "Button 1 mapped to joystick button " << emu_opt.joy1 << endl;
			}
			else
			{
			        cerr << "No valid button for button 1 was given. Using default value." << endl;
	  	            emu_opt.joy1 = 0;
			}
        }

        if (strcmp(rv[i],"-joy2")==0)
        {
		   if (i < a-1)
	 	   {
	           emu_opt.joy2 = atoi( rv[i+1]);
				if (emu_opt.joy2 < 0)
				{
			        cerr << "No valid button for button 2 was given. Using default value." << endl;
	  	            emu_opt.joy2 = 1;
				}
				cout << "Button 2 mapped to joystick button " << emu_opt.joy2 << endl;
			}
			else
			{
			        cerr << "No valid button for button 2 was given. Using default value." << endl;
	  	            emu_opt.joy2 = 1;
			}
        }

        if (strcmp(rv[i],"-joystart")==0)
        {
		   if (i < a-1)
	 	   {
	           emu_opt.joystart = atoi( rv[i+1]);
				if (emu_opt.joystart < 0)
				{
			        cerr << "No valid button for button Start was given. Using default value." << endl;
	  	            emu_opt.joystart = 2;
				}
				cout << "Button Start mapped to joystick button " << emu_opt.joystart << endl;
			}
			else
			{
			        cerr << "No valid button for button A was given. Using default value." << endl;
	  	            emu_opt.joystart = 2;
			}
        }


        if (strcmp(rv[i],"-dp")==0)
        {
            emu_opt.bright_palette = false;
        }

        // The ini file MUST be the next parameter after -inifile option.
        if (strcmp(rv[i],"-inifile")==0)
        {
		    // If actual argument nbr < argument number eg is there any args ?
		    if (i < a-1)
		    {
	            emu_opt.default_config = false;
		        strncpy(emu_opt.ini_file, rv[++i],512);
		    }
		    else
		    {
		        cerr << "No ini file was given, using default configuration." << endl;
	            emu_opt.default_config = true;
		    }
        }

        char extension [][5] = {".zip",".ZIP",".sms",".SMS",".gg",".GG"};
        for (int o = 0; o < 6; o++)
        {
                char *pattern = NULL;
                pattern = strstr(rv[i], extension[o]);
                if (pattern != NULL)
                {
                    rom_filename_index = i;
                }
        }

    }
    return(rom_filename_index);
}

/*--------------------------------------------------------------------*/
/* This method writes Osmose splash screen, version, author and build */
/* informations.						      */
/*--------------------------------------------------------------------*/
void splashText()
{
    cout << endl <<  __OSMOSE_VERSION__ <<" build on " << __DATE__<< " at " << __TIME__<< " Written by Bruno Vedder." << endl;
}


           _______                  _______
          |       |.-----.--------.|       |.-----.-----.
          |   -   ||__ --|        ||   -   ||__ --|  -__|
          |_______||_____|__|__|__||_______||_____|_____|
           _______                  __         __
          |    ___|.--------.--.--.|  |.---.-.|  |_.-----.----.
          |    ___||        |  |  ||  ||  _  ||   _|  _  |   _|
          |_______||__|__|__|_____||__||___._||____|_____|__|
       Version 0.8.0 - 8th public release, written by Bruno Vedder.


Osmose means:
-------------

Object      Oriented
       S.m.s.         Emulator.

In brief it's an Sega Master System / Game Gear emulator encapsulated into C++
classes :-)


24 December 2006:  Osmose-0-8-1 9th public release.
--------------------------------------------------

Osmose is another Sega Master System / Gamegear emulator. The emulator is
released as open source project, under GPL license. For more information just
read the file 'license.txt'.

The emulator relies on SDL library and Zlib and Gilles Vollant unzip package.
Scale2x video Filter is based on Andrea Mazzoleni's algorithm.

What's new:
-----------

-Add Save state. User can save in 5 different slot emulation state
 and reload them later. Use keypad + - to change slot. F11 Load a state,
 F12 save a state in current slot.

Features:
--------

-SMS: Good compatibility. At this stage, the emulator can run* 96% of commercial
 games and public demos, except games that relies on codemaster mapper, which work
 but does not have proper video mode emulated.
-Game Gear: Good compatibility. At this stage, the emulator can run 98.0%* of
 game gear ROMS.
-SN76489 Sound is supported, but needs more work on noise register.
-support. for .zip .sms  and .gg format.
-Video filters: monochrom, scanline, scale2x, bilinear, nearest neighbour,
 and a new interesting scale2xscanline (fullscreen option always available).
-Pad(keyboard or joystick mapped), Sport Paddle (mouse mapped) emulation.
-PAL/NTSC Timing.
-Japanese/Export console.
-In game Screenshots, GFX rip, sound shot.
-Accurate auto-frameskip.
-Configurable keyboard configuration.
-Joystick support.

*Due to the huge number of game gear/sms (around 1300) roms, games have not been
deeply tested.

About scale2xscanline video filter: I was pleased by the scale2x renderer except
for it's brightness, to high for me. But i liked the rounded shapes instead of big
square pixels. That's why i wrote an alternate video filter which apply scale2x AND
scanline video filter. I find the result really interesting.

I finally add 2x Nearest neighbour video filter because some hardcore emulator users
don't accept (in term of emulation) screen modifier saying: real SMS does not have
bilinear video filtering. That's a point of view that i understand. So i add this
basic and pixel harmless video filter :)

Usage:
------

./osmose rom_name.zip -tv
./osmose test.sms -nosound -fs -tv
./osmose crazy_game.gg -inifile my_conf.ini

Notes:
ROM name MUST be the first parameter.
If you are not easy with command line, maybe should you use 'wxosmose', which
is a graphical frontend for Osmose. It's written by Bertram, thanks to him :-)


Options:
--------

Osmose 0-8-0 now implements embedded database, with game that needs specific
options to turn them on autmatically. User don't need to use -cm, -km -pal
or -jap options. But i don't own all roms, that why these option are still
usable from command line.

    -paddle           emulates one axis paddle (mapped on mouse).
    -joy              use joystick as input device, instead of keyboard.
    -acceleration x.x paddle acceleration (0.1 to 5 default 0.5)
    -fs               run in fullscreen   (default: windowed).
    -nosound          do not play sounds. (default: sound on).
    -dp               use dark palette for screen emulation (default: off).
    -inifile          xxx use xxx as configuration file.
    -fps              display fps in title bar.
    -monochrom        emulates B&W tv (default: off).
    -scale2x          scale2x video filter implementation (default: off).
    -bilinear         bilinear video filter (default: off).
    -tv               emulates TV scanline video filter (default: off).
    -scale2xscanline  Scale2x + 75% scanline video filter (default: off).
    -nn2x             nearest neighbour video filter (default: off).
    -cm               use codemaster games mem. mapper (default: off).
    -km               use korean games mem. mapper (default: off).
    -irqhack          Enable irq hack (specific rom option. default off).
    -pal              emulates PAL/SECAM video timing (default: NTSC).
    -jap              run as japanese sms (default: export).
    -exp              run as exported sms (default).

Important notes:

       -You can combine -tv/-monochrom/-scale2x with -fs (fullscreen).
       -If your config file name contains some spaces chars, use -inifile
        like this: -inifile "Name with space.txt" (with double quotes).

Default Keys:
-------------

SOUNDSHOT :  F1 (start/stop audio recording (22050kz 16bits/mono into wav file)
             Be carefull, wave file grows about 2.58 Mo/minute.
PAUSE     :  p
SCREENSHOT:  F2
TILESHOT  :  F3  (this will rip gfx used at this moment).
QUIT      :  ESCAPE
[DEBUGGER : d, if Osmose compiled with builtin debugger]

PAD1 UP   :  UP
PAD1 DOWN :  DOWN
PAD1 LEFT :  LEFT
PAD1 RIGHT:  RIGHT
PAD1 A    :  LEFT ALT
PAD1 B    :  LEFT CONTROL

PAD2 UP   :  NUMPAD 5
PAD2 DOWN :  NUMPAD 2
PAD2 LEFT :  NUMPAD 1
PAD2 RIGHT:  NUMPAD 3
PAD2 A    :  n
PAD2 B    :  b

ENTER     :  Game Gear start button.

KEYPAD   +:  Increment save state slot.
KEYPAD   -:  Decrement save state slot.
F11       :  Load a state from current slot.
F12       :  Save a state in current slot.

Keyboard configuration:
-----------------------

If you dislike Osmose's keyboard configuration, you can change it,
by providing a file with new configuration. When run, osmose will create
a osmose.ini file with default configuration. Under Linux/Unix this file
is created in ./osmose of user directory. You can specify other configuration
file with '-inifile my_config.txt' option

Sound shots, tiles rip and screenshots, Battery backed ram:
---------------------------------------

Osmose save these files in the corresponding folders:
./snd
./tiles
./screen
./bbr
./saves

If the emulator doesn't find osmose.ini, it will generate a new one, with
these four folders. Without these folders no save are possible. If Osmose
can't create them do it manually !

To force osmose to create these directory again, simply delete osmose.ini
and launch a ROM.

Building Osmose from sources:
-----------------------------

Assuming your system is correctly configured, with SDL library,
just type: make from the source directory.

Thanks to:
----------
-Enik, for bug reports, beta testing, and sound comments.

-Bertram, for the Osmose's Frontend, based on wxWidgets, and bug reports.

-Alessandro Scotti, for CPU core, and nice information about it.
 Web site: http://ascotti.org/programming/tickle/tickle.htm

-Charles MacDonald, for his excellent technical documentation on the SMS
 hardware.
 Web site: http://cgfm2.emuviews.com/

-SMS Power dev forum, for help about a lot of SMS parts, and nice resources.
 Web site: http://www.smspower.org/

-Zlib home page for their excellent library.
 Web site: http://www.gzip.org/zlib

-Gilles Vollant for it's unzip package based on zlib.

-emu-france.com for free and publicity free web hosting.
 Web site: http://emu-france.com

-Andrea Mazzoleni, for providing scale2x video filter algorithm.
 Web site: http://scale2x.sourceforge.net/

-Sound beta tester from 6t-web.
 Web site: http://6t-web.com

Feedback for bugs, problems or features requests are obiously welcome.

--------------------------------------------------------------------
|  Contact: osmose_emulator@yahoo.fr                               |
|  Site   : http://bcz.emu-france.com                              |
--------------------------------------------------------------------

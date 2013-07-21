/*****************************************************************************
*
* File: JoystickInputDevice.cpp
*
* Project: Osmose emulator.
*
* Description: This class implements input device Pad.
*
* Author: Vedder Bruno
* Date: 14/11/2006, 15h50
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "JoystickInputDevice.h"

extern EmulatorOptions emu_opt;

/*--------------------------------------------------------------------*/
/* Class constructor.                                                 */
/*--------------------------------------------------------------------*/
JoystickInputDevice::JoystickInputDevice(IOMapper *iomapper, OsmoseConfiguration *o)
{
    int joy_num = 0;
    iom = iomapper;
    oc = o;
	joy_num = SDL_NumJoysticks();
    if (joy_num == 0)
	{
        cerr << "No Joystick found. Try to connect or configure it and then restart Osmose." << endl;
	    exit(-1);
	}
    cout << "Joystick: " << SDL_JoystickName(0) <<endl;
    SDL_JoystickEventState(SDL_ENABLE);
    j = SDL_JoystickOpen(0);

}

/*---------------------------------------------------------------------*/
/* Keyboard events are used to emulate DEFAULT PAD                     */
//*--------------------------------------------------------------------*/
void JoystickInputDevice::handleDeviceChange(SDL_Event &e)
{
    int type = e.type;
	
	/* Handle Joystick Motion */
    if (type == SDL_JOYAXISMOTION)
    {
		if ( e.jaxis.axis == 0)
        {
 	       /* Left/Right Axis is in central position. */
			if (e.jaxis.value > -3200 && e.jaxis.value < 3200)
			{
	            iom->portPAD1 |= BIT3;
	            iom->portPAD1 |= BIT2;
			}

			if (e.jaxis.value < -3200)
			{
				/* Left movement avoid left and right pad flag to be set together :) */
				iom->portPAD1 &= BIT2_MASK;
	            iom->portPAD1 |= BIT3;
				return;
			}
			if (e.jaxis.value > 3200)
			{
				/* Right movement avoid left and right pad flag to be set together :) */
				iom->portPAD1 &= BIT3_MASK;
	            iom->portPAD1 |= BIT2;
				return;
			}
            return;
        }

        /* Up/Down Axis is in central position. */
        if( e.jaxis.axis == 1) 
        {
			if (e.jaxis.value > -3200 && e.jaxis.value < 3200)
			{
	            iom->portPAD1 |= BIT0;
	            iom->portPAD1 |= BIT1;
			}

			if (e.jaxis.value < -3200)
			{
				/* Down movement avoid up and down pad flag to be set together :) */
				iom->portPAD1 &= BIT0_MASK;
	            iom->portPAD1 |= BIT1;
				return;
			}
			if (e.jaxis.value > 3200)
			{
				/* Down movement avoid up and down pad flag to be set together :) */
				iom->portPAD1 &= BIT1_MASK;
	            iom->portPAD1 |= BIT0;
				return;
			}
			return;
        }
    }

	/* Handle Buttons events.*/
    if (type == SDL_JOYBUTTONDOWN)
    {

    	if ( (int)e.jbutton.button == emu_opt.joy1 ) 
    	{
            iom->portPAD1 &= BIT4_MASK;
    		return;
    	}
    	if ( (int)e.jbutton.button == emu_opt.joy2 ) 
    	{
            iom->portPAD1 &= BIT5_MASK;
    		return;
    	}
    	if ( (int)e.jbutton.button == emu_opt.joystart ) 
    	{
            iom->port0x0 &= BIT7_MASK;
    		return;
    	}
		return;
    }

    if (type == SDL_JOYBUTTONUP)
    {

    	if ( (int)e.jbutton.button == emu_opt.joy1 ) 
    	{
            iom->portPAD1 |= BIT4;
    		return;
    	}
    	if ( (int)e.jbutton.button == emu_opt.joy2 ) 
    	{
            iom->portPAD1 |= BIT5;
    		return;
    	}
    	if ( (int)e.jbutton.button == emu_opt.joystart ) 
    	{
            iom->port0x0 |= BIT7;
    		return;
    	}
		return;
    }
}

/*----------------------------------------------------------------------*/
/* Unused in this device. Use when emulating analogic thing on keyboard.*/
/*----------------------------------------------------------------------*/
void JoystickInputDevice::updateDevice()
{
}

/*----------------------------------------------------------------------*/
/* Reset the device.                                                    */
/*----------------------------------------------------------------------*/
void JoystickInputDevice::reset()
{
}

/*----------------------------------------------------------------------*/
/* Return ID string.                                                    */
/*----------------------------------------------------------------------*/
string JoystickInputDevice::getInputDeviceName()
{
    return string("Joystick");
}

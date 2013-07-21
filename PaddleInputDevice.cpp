/*****************************************************************************
*
* File: PadInputDevice.cpp
*
* Project: Osmose emulator.
*
* Description: This class implements input device Pad.
*
* Author: Vedder Bruno
* Date: 19/04/2005, 18h30
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#include "PaddleInputDevice.h"

/*--------------------------------------------------------------------*/
/* Class constructor.                                                 */
/*--------------------------------------------------------------------*/
PaddleInputDevice::PaddleInputDevice(IOMapper *iomapper, OsmoseConfiguration *o)
{
    iom = iomapper;
    oc = o;
    increment  = 0;
    acceleration = opt.acceleration;
}

/*--------------------------------------------------------------------*/
/* Mouse events are used to emulate ONE AXIS PADDLE                   */
/* Button A and B are LEFT mouse button, and right mouse button.      */
/*--------------------------------------------------------------------*/
void PaddleInputDevice::handleDeviceChange(SDL_Event &e)
{
    int type = e.type;
    short v;
        
    if (type == SDL_MOUSEMOTION)
    {
        increment +=(e.motion.xrel* acceleration);
	v = (short) increment;
	if (v >0xFF) v = 0xFF;
        if (v < 0x0) v = 0x0;
	iom->paddleValue = (unsigned char) v;
    }
    else
    {
	if (type == SDL_MOUSEBUTTONDOWN)
	{
	    if ( e.button.button == SDL_BUTTON_LEFT)
	    {
        	    iom->portPAD1 &= BIT4_MASK;
		    return;
	    } 
	}
	if (type == SDL_MOUSEBUTTONUP)
	{
	    if ( e.button.button == SDL_BUTTON_LEFT)
	    {
                    iom->portPAD1 |= BIT4;
		    return;
	    } 
	}
    }
}

/*----------------------------------------------------------------------*/
/* Unused in this device. Use when emulating analogic thing on keyboard.*/
/*----------------------------------------------------------------------*/
void PaddleInputDevice::updateDevice()
{
}

/*----------------------------------------------------------------------*/
/* Reset the device.                                                    */
/*----------------------------------------------------------------------*/
void PaddleInputDevice::reset()
{
    increment  = 0;
}

/*----------------------------------------------------------------------*/
/* Return ID string.                                                    */
/*----------------------------------------------------------------------*/
string PaddleInputDevice::getInputDeviceName()
{
    return string("ONE AXIS PADDLE");
}

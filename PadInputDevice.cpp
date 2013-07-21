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
#include "PadInputDevice.h"

/*--------------------------------------------------------------------*/
/* Class constructor.                                                 */
/*--------------------------------------------------------------------*/
PadInputDevice::PadInputDevice(IOMapper *iomapper, OsmoseConfiguration *o)
{
    iom = iomapper;
    oc = o;
}

/*---------------------------------------------------------------------*/
/* Keyboard events are used to emulate DEFAULT PAD                     */
//*--------------------------------------------------------------------*/
void PadInputDevice::handleDeviceChange(SDL_Event &e)
{
    int type = e.type;
    int k = e.key.keysym.sym;

    if (type == SDL_KEYDOWN)
    {
    	if ( k == oc->P1_DOWN_KEY)
    	{
            iom->portPAD1 &= BIT1_MASK;
    		return;
    	}

    	if ( k == oc->P1_UP_KEY)
    	{
            iom->portPAD1 &= BIT0_MASK;
    		return;
    	}

    	if ( k == oc->P1_LEFT_KEY)
    	{
            iom->portPAD1 &= BIT2_MASK;
    		return;
    	}

    	if ( k == oc->P1_RIGHT_KEY)
    	{
            iom->portPAD1 &= BIT3_MASK;
    		return;
    	}

    	if ( k == oc->P1_A_KEY)
    	{
            iom->portPAD1 &= BIT4_MASK;
    		return;
    	}

    	if ( k == oc->P1_B_KEY)
    	{
            iom->portPAD1 &= BIT5_MASK;
    		return;
    	}

    	if ( k == oc->P2_DOWN_KEY)
    	{
            iom->portPAD1 &= BIT7_MASK;
    		return;
    	}

    	if ( k == oc->P2_UP_KEY)
    	{
            iom->portPAD1 &= BIT6_MASK;
    		return;
    	}

    	if ( k == oc->P2_LEFT_KEY)
    	{
            iom->portPAD2 &= BIT0_MASK;
    		return;
    	}

    	if ( k == oc->P2_RIGHT_KEY)
    	{
            iom->portPAD2 &= BIT1_MASK;
    		return;
    	}

    	if ( k == oc->P2_A_KEY)
    	{
            iom->portPAD2 &= BIT2_MASK;
    		return;
    	}

    	if ( k == oc->P2_B_KEY)
    	{
            iom->portPAD2 &= BIT3_MASK;
    		return;
    	}

    	if ( k == oc->GG_START_KEY)
    	{
            iom->port0x0 &= BIT7_MASK;
	    	return;
    	}
    }

    if (type == SDL_KEYUP)
    {
        if ( k == oc->P1_DOWN_KEY)
    	{
            iom->portPAD1 |= BIT1;
    		return;
        }

        if ( k == oc->P1_UP_KEY)
    	{
            iom->portPAD1 |= BIT0;
    		return;
        }

        if ( k == oc->P1_LEFT_KEY)
    	{
            iom->portPAD1 |= BIT2;
    		return;
        }

        if ( k == oc->P1_RIGHT_KEY)
    	{
            iom->portPAD1 |= BIT3;
    		return;
        }

        if ( k == oc->P1_A_KEY)
    	{
            iom->portPAD1 |= BIT4;
    	    return;
        }

        if ( k == oc->P1_B_KEY)
    	{
            iom->portPAD1 |= BIT5;
    		return;
        }

        if ( k == oc->P2_DOWN_KEY)
    	{
            iom->portPAD1 |= BIT7;
    		return;
        }

        if ( k == oc->P2_UP_KEY)
    	{
            iom->portPAD1 |= BIT6;
    		return;
        }

        if ( k == oc->P2_LEFT_KEY)
    	{
            iom->portPAD2 |= BIT0;
    		return;
        }

        if ( k == oc->P2_RIGHT_KEY)
    	{
            iom->portPAD2 |= BIT1;
    		return;
        }

        if ( k == oc->P2_A_KEY)
    	{
            iom->portPAD2 |= BIT2;
    		return;
        }

        if ( k == oc->P2_B_KEY)
    	{
            iom->portPAD2 |= BIT3;
            return;
        }

    	if ( k == oc->GG_START_KEY)
    	{
            iom->port0x0 |= BIT7;
    		return;
    	}
    }
}

/*----------------------------------------------------------------------*/
/* Unused in this device. Use when emulating analogic thing on keyboard.*/
/*----------------------------------------------------------------------*/
void PadInputDevice::updateDevice()
{
}

/*----------------------------------------------------------------------*/
/* Reset the device.                                                    */
/*----------------------------------------------------------------------*/
void PadInputDevice::reset()
{
}

/*----------------------------------------------------------------------*/
/* Return ID string.                                                    */
/*----------------------------------------------------------------------*/
string PadInputDevice::getInputDeviceName()
{
    return string("Default PAD");
}

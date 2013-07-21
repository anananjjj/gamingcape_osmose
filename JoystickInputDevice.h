/*****************************************************************************
*
* File: JoystickInputDevice.h
*
* Project: Osmose emulator.
*
* Description: This class handles input device through Joystick.
*
* Author: Vedder Bruno
* Date: 14/11/2006, 15h50
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#ifndef JOYSTICK_INPUT_DEVICE_H
#define JOYSTICK_INPUT_DEVICE_H

#include "InputDevice.h"
#include "IOMapper.h"
#include "OsmoseConfiguration.h"
#include "Options.h"

class JoystickInputDevice: public InputDevice
{

public:
    IOMapper *iom;
    OsmoseConfiguration *oc;

    JoystickInputDevice(IOMapper *, OsmoseConfiguration *);
    virtual ~JoystickInputDevice() {};
    void handleDeviceChange(SDL_Event &);
    void updateDevice();
    void reset();
    string getInputDeviceName();

private:
	SDL_Joystick *j;
};

#endif

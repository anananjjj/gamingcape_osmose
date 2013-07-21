/*****************************************************************************
*
* File: InputDevice.h
*
* Project: Osmose emulator.
*
* Description: This abstract class define a common interface for input devices
* like Pad, Trackball, Paddle.
*
* Real device should inherit this class and provide implementation.
*
* Author: Vedder Bruno
* Date: 19/04/2005, 18h30
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/
#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <SDL/SDL.h>
#include <iostream>

using namespace std;

class InputDevice
{

public:
    virtual ~InputDevice() {};
    virtual void handleDeviceChange(SDL_Event &) = 0;
    virtual void updateDevice() = 0;
    virtual void reset() = 0;
    virtual string getInputDeviceName() = 0;
};

#endif

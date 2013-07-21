/*****************************************************************************
*
* File: PadInputDevice.h
*
* Project: Osmose emulator.
*
* Description: This class handles input device Pad.
*
* Author: Vedder Bruno
* Date: 19/04/2005, 18h30
*
* URL: http://bcz.emu-france.com/
*****************************************************************************/

#ifndef PADDLE_INPUT_DEVICE_H
#define PADDLE_INPUT_DEVICE_H

#include "InputDevice.h"
#include "IOMapper.h"
#include "OsmoseConfiguration.h"

#define MAX_INCREMENT    10
#define MIN_INCREMENT     3

class PaddleInputDevice: public InputDevice
{

public:
    IOMapper *iom;
    OsmoseConfiguration *oc;

    PaddleInputDevice(IOMapper *, OsmoseConfiguration *);
    virtual ~PaddleInputDevice() {};
    void handleDeviceChange(SDL_Event &);
    void updateDevice();
    void reset();
    string getInputDeviceName();

private:
    float increment;               // increment.
    float acceleration;		   // Acceleration factor.
};

#endif

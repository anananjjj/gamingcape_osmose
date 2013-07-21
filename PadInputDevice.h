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

#ifndef PAD_INPUT_DEVICE_H
#define PAD_INPUT_DEVICE_H

#include "InputDevice.h"
#include "IOMapper.h"
#include "OsmoseConfiguration.h"

class PadInputDevice: public InputDevice
{

public:
    IOMapper *iom;
    OsmoseConfiguration *oc;

    PadInputDevice(IOMapper *, OsmoseConfiguration *);
    virtual ~PadInputDevice() {};
    void handleDeviceChange(SDL_Event &);
    void updateDevice();
    void reset();
    string getInputDeviceName();
};

#endif

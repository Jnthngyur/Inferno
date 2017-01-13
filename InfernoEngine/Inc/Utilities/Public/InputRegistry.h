// InputRegistr.h
// Author: Jonathan Gyurkovics
// Purpose: Handling registering and unregistering of delegates to requested input events

#ifndef _INPUTREGISTRY_H
#define _INPUTREGISTRY_H

#include "InputVals.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <functional>

// === TypeDefs
typedef std::function<void(INPUT_STATUS)> ActionEventFunction;
typedef std::function<void(float)> AxisEventFunction;
typedef std::function<void(POINT, POINT)> MouseMovementEventFunction;
typedef std::function<void(unsigned int)> MouseWheelEventFunction;
typedef std::function<void(bool)> ControlChangeEventFunction;
typedef std::function<void(unsigned int)> ControllerLostEventFunction;
typedef std::function<void(unsigned int)> ControllerGainedEventFunction;

class InputRegistry
{
public:
	InputRegistry();
	~InputRegistry();
};

#endif
// Globals.h
// Author: Jonathan Gyurkovics
// Purpose: Contains all structs/enums/variables which all files can have access to

#ifndef _GLOBALS_H
#define _GLOBALS_H

// Some global defines
#define SAFE_DELETE(x) if(x) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete[] x; x = nullptr; }

// Include the standard int library for protability
#include <cstdint>

enum class WindowMode {
	Windowed = 0,
	Borderless = 1,
	Fullscreen = 2,
};

#endif
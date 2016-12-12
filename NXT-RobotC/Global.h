#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#define ARDUINO_ADDRESS 0x08
ubyte leftDist = 0x42;
ubyte frontDist = 0x43;
ubyte rightDist = 0x44;
ubyte downLeftDist = 0x45;
ubyte downRightDist = 0x46;

typedef enum eColor {
	cInvalid = 255,
	cGreen = 2,
	cSilver = 4,
	cWhite = 0,
	cBlack = 1,
	cGradient = 3
} Color;

float getDistance(ubyte sensor)
{
	if (sensor < 0x42 || sensor > 0x46)
		return -1;
	long ret = 0;
	return (float)ret/1000;
}

Color getColor(ubyte sensor)
{
	if (sensor < 0x47)
		return cInvalid;
	return cWhite;
}

#endif

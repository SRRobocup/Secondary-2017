#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#define ARDUINO_ADDRESS 0x08
tSensors arduino = S4;
ubyte leftDist = 0x42;
ubyte frontDist = 0x43;
ubyte rightDist = 0x44;
ubyte downLeftDist = 0x45;
ubyte downRightDist = 0x46;
const int numOfIterations = 31;
const float microsecondsPerIteration = (float) 1000 / numOfIterations;

typedef enum eColor {
	cInvalid = 255,
	cGreen = 2,
	cSilver = 4,
	cWhite = 0,
	cBlack = 1,
	cGradient = 3
} Color;

typedef struct sColorSensor {
	ubyte address;
	float blackThreshold;
	float whiteThreshold;
	float greenRatio;
	bool isLight;
} ColorSensor;

void delayMicroseconds(int num)
{
	for (int i = 1; i*microsecondsPerIteration < num; i++)
		noOp();
}

float getDistance(ubyte sensor)
{
	if (sensor < 0x42 || sensor > 0x46)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor;
	writeI2C(arduino,send);
	delayMicroseconds(40);
	send[2] = 0;
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	if (ret == 32767)
		return -1;
	return (float)ret/1000;
}

Color getColor(ColorSensor sensor)
{
	if (sensor.address < 0x47 || sensor.address > 0x54)
		return cInvalid;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	delayMicroseconds(40);
	send[2] = 0;
	writeI2C(arduino,send,receive,8);
	int red = receive[7] << 8 | receive[6];
	int blue = receive[5] << 8 | receive[4];
	int green = receive[3] << 8 | receive[2];
	int clear = receive[1] << 8 | receive[0];
	if (sensor.isLight)
		return (Color) (clear < sensor.blackThreshold);
	if (green < sensor.blackThreshold)
		return cBlack;
	if (green > sensor.whiteThreshold)
		return cWhite;
	if ((float)green/red > sensor.greenRatio)
		return cGreen;
	return cGradient;
}

#endif

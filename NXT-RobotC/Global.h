#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#define ARDUINO_ADDRESS 0x08

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
	float bwThreshold;
	float silverThreshold;
	bool isLight;
	Color currentColor;
} ColorSensor;

typedef struct sLaserSensor {
	ubyte address;
} LaserSensor;

typedef struct sPingSensor {
	ubyte address;
} PingSensor;

typedef tSensors SensorPort;
typedef tMotor MotorPort;

SensorPort arduino = S4;
MotorPort LMotor;
MotorPort RMotor;
LaserSensor leftDist;
LaserSensor frontDist;
LaserSensor rightDist;
LaserSensor downLeftDist;
LaserSensor downRightDist;
PingSensor frontPing;
ColorSensor leftFrontL;
ColorSensor leftFrontM;
ColorSensor middleFront;
ColorSensor rightFrontR;
ColorSensor rightFrontM;
const int numOfIterations = 31;
const float obstacleThreshold = 7;
const float microsecondsPerIteration = (float) 1000 / numOfIterations;

void generateColor(ColorSensor* sensor, ubyte address,float bwThreshold,float silverThreshold,float blackThreshold, float whiteThreshold, float greenRatio, bool isLight)
{
	sensor->address = address;
	sensor->bwThreshold = bwThreshold;
	sensor->blackThreshold = blackThreshold;
	sensor->whiteThreshold = whiteThreshold;
	sensor->greenRatio = greenRatio;
	sensor->isLight = isLight;
	sensor->currentColor = cInvalid;
}

void delayMicroseconds(int num)
{
	for (int i = 1; i*microsecondsPerIteration < num; i++)
		noOp();
}

float getDistance(LaserSensor sensor)
{
	if (sensor < 0x42 || sensor > 0x46)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	delayMicroseconds(40);
	send[2] = 0;
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	if (ret == 32767)
		return -1;
	return (float)ret/1000;
}

float getDistance(PingSensor sensor)
{
	if (sensor != 0x47)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	delayMicroseconds(40);
	send[2] = 0;
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	return (float)ret / 29 / 2;
}

void getColorRGB(ColorSensor sensor, int& r, int& g, int&b, int& clear)
{
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	delayMicroseconds(40);
	send[2] = 0;
	writeI2C(arduino,send,receive,8);
	r = receive[7] << 8 | receive[6];
	g = receive[5] << 8 | receive[4];
	b = receive[3] << 8 | receive[2];
	clear = receive[1] << 8 | receive[0];
}

Color getColor(ColorSensor sensor)
{
	if (sensor.address < 0x48 || sensor.address > 0x55)
		return cInvalid;
	int red, green, blue, clear;
	getColorRGB(sensor,red,green,blue,clear);
	if (sensor.isLight)
		if (clear > sensor.silverThreshold)
			return cSilver;
		return (Color) (clear < sensor.bwThreshold);
	if (green < sensor.blackThreshold)
		return cBlack;
	if (green > sensor.whiteThreshold)
		return cWhite;
	if ((float)green/red > sensor.greenRatio)
		return cGreen;
	return cGradient;
}

#endif

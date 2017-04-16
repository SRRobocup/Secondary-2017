#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#include "mindsensors-lightsensorarray.h"
#include "C:\Users\ethan_000\Documents\Git Repos\RCJ Secondary 2017\Register.h"
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
	int clear;
	int green;
} ColorSensor;

typedef struct sLaserSensor {
	ubyte address;
} LaserSensor;

typedef struct sPingSensor {
	ubyte address;
} PingSensor;

typedef tSensors SensorPort;
typedef tMotor MotorPort;

SensorPort arduino;
MotorPort LMotor;
MotorPort RMotor;
LaserSensor leftDist;
LaserSensor frontDist;
LaserSensor rightDist;
PingSensor frontPing;
ColorSensor leftFrontL;
ColorSensor leftFrontM;
ColorSensor middleFront;
ColorSensor rightFrontR;
ColorSensor rightFrontM;
SensorPort MSLSA;
int forward = 30;
int turnForward = 55;
int turnBackward = -10;
const float wheelbase = 15.7;
const float width = 13.5;
const float diameter = 3;
const float cm = 360.0 / (diameter *  PI);
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

float clamp(float val,float up,float low)
{
	if(val > up)
		return up;
	if(val < low)
		return low;
	return val;
}

void delayMicroseconds(int time, int iters = 1000)
{
	if (time == -1)
		time = 30000000;
	for (int i = 1; i*microsecondsPerIteration < time && i < iters; i++)
		noOp();
}

bool stillRunning(MotorPort m)
{
#ifdef NXT
	return nMotorRunState[m] != runStateIdle;
#else
	return getMotorRunning(m);
#endif
}

void resetEncoders()
{
	nMotorEncoder[LMotor] = 0;
	nMotorEncoder[RMotor] = 0;
}

void stopMotors()
{
	motor[LMotor] = 0;
	motor[RMotor] = 0;
}

// turning the robot right a certain amount of degrees
void turnRight(float deg, int power = forward)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	nMotorEncoderTarget[LMotor] = target;
	motor[LMotor] = power;
	motor[RMotor] = -power;
	while(stillRunning(LMotor)){}
	stopMotors();
	wait1Msec(100);
}

void turnLeft(float deg, int power = forward)
{
	float target = wheelbase*PI*(deg/360)*cm;
	resetEncoders();
	nMotorEncoderTarget[RMotor] = target;
	motor[LMotor] = -power;
	motor[RMotor] = power;
	while(stillRunning(RMotor)){}
	stopMotors();
	wait1Msec(100);
}

void goStraight(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	nMotorEncoderTarget[LMotor] = dist;
	motor[LMotor] = power;
	motor[RMotor] = power;
	while(stillRunning(LMotor)){}
	stopMotors();
}

void goBack(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	nMotorEncoderTarget[LMotor] = -dist;
	motor[LMotor] = -power;
	motor[RMotor] = -power;
	while(stillRunning(LMotor)){}
	stopMotors();
}

float getDistance(LaserSensor sensor)
{
	if (sensor < LEFT_LASER || sensor > RIGHT_LASER)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	wait1Msec(2);
	send[2] = 0;
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	if (ret == 32767)
		return -1;
	return (float)ret/10;
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
	send[2] = 0x00;
	send[3] = 0x03;
	delay(2);
	delayMicroseconds(-1,1);
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	return (float)ret / 29 / 2;
}

void getColorRGB(ColorSensor sensor, int& r, int& g, int& b)
{
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
#ifdef DEBUG
	writeDebugStreamLine("Time Start: %d", nPgmTime);
#endif
	wait1Msec(2);
#ifdef DEBUG
	writeDebugStreamLine("Time End: %d", nPgmTime);
#endif
	send[2] = 0x00;
	send[3] = 0x03;
	writeI2C(arduino,send,receive,8);
	r = receive[7] << 8 | receive[6];
	b = receive[5] << 8 | receive[4];
	g = receive[3] << 8 | receive[2];
	sensor.green = g;
	sensor.clear = receive[1] << 8 | receive[0];
}

void getColorRGB(ColorSensor sensor, int& r, int& g, int& b, int& c)
{
	getColorRGB(sensor,r,g,b);
	c = sensor.clear;
	//clearI2CError(arduino, ARDUINO_ADDRESS);
}

Color getColor(ColorSensor sensor)
{
	do {
		if (sensor.address < LEFT_FRONT || sensor.address > RIGHT_FRONT)
		{
			sensor.currentColor = cInvalid;
			break;
		}
		int red,green,blue;
		getColorRGB(sensor,red,green,blue);
		if (sensor.isLight)
		{
			//if (sensor.clear > sensor.silverThreshold)
			//	return cSilver;
			//else
				sensor.currentColor = (Color) (sensor.clear < sensor.bwThreshold);
				break;
		}
		if (green < sensor.blackThreshold)
		{
			sensor.currentColor = cBlack;
			break;
		}
		if (green > sensor.whiteThreshold)
		{
			sensor.currentColor = cWhite;
			break;
		}
		if ((float)green/red > sensor.greenRatio)
		{
			sensor.currentColor = cGreen;
			break;
		}
		writeDebugStreamLine("GRADIENT %d: %d %d", sensor.address, green, red);
		sensor.currentColor = cGradient;
	} while (false);
	return sensor.currentColor;
}

bool seeBlackArray()
{
	bool ret = false;
	ubyte threshold = 40;
	ubyte values[8];
	MSLSAreadSensors(MSLSA,values);
	for (int i = 0; i < 8; i++)
	{
		ret = values[i] < threshold || ret;
	}
	//for (int i = 0; i < 8; i++)
	//		writeDebugStream("%d ", values[i]);
	//	writeDebugStreamLine("");
	return ret;
}

bool seeLine()
{
	Color leftL = getColor(leftFrontL);
	Color leftM = getColor(leftFrontM);
	Color middle = getColor(middleFront);
	Color rightM = getColor(rightFrontM);
	Color rightR = getColor(rightFrontR);
	//writeDebugStreamLine("SEELINE: %d %d %d %d %d", leftL, leftM, middle, rightM, rightR);
	return (leftL != cWhite || leftM != cWhite || middle != cWhite || rightM != cWhite || rightR != cWhite);
}

void suspend()
{
	stopMotors();
	while(true){}
}

#endif

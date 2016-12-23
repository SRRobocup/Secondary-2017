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
	int red;
	int green;
	int blue;
	int clear;
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
int forward = 30;
int turnForward = 20;
int turnBackward = -15;
const float wheelbase = 19;
const float width = 17;
const float diameter = 4.5;
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
	while(stillRunning(LMotor) || stillRunning(RMotor)){}
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
	while(stillRunning(RMotor) || stillRunning(LMotor)){}
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
	if (sensor < 0x42 || sensor > 0x46)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	send[0] = 2;
	send[1] = ARDUINO_ADDRESS;
	send[2] = sensor.address;
	writeI2C(arduino,send);
	delayMicroseconds(-1,1);
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
	delayMicroseconds(-1,1);
	send[2] = 0;
	writeI2C(arduino,send,receive,2);
	ret = receive[1] << 8 | receive[0];
	return (float)ret / 29 / 2;
}

void getColorRGB(ColorSensor sensor)
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
	sensor.red = receive[7] << 8 | receive[6];
	sensor.green = receive[5] << 8 | receive[4];
	sensor.blue = receive[3] << 8 | receive[2];
	sensor.clear = receive[1] << 8 | receive[0];
}

Color getColor(ColorSensor sensor)
{
	if (sensor.address < 0x48 || sensor.address > 0x55)
		return cInvalid;
	getColorRGB(sensor);
	if (sensor.isLight)
		if (sensor.clear > sensor.silverThreshold)
			return cSilver;
		return (Color) (sensor.clear < sensor.bwThreshold);
	if (sensor.green < sensor.blackThreshold)
		return cBlack;
	if (sensor.green > sensor.whiteThreshold)
		return cWhite;
	if ((float)sensor.green/sensor.red > sensor.greenRatio)
		return cGreen;
	return cGradient;
}

#endif

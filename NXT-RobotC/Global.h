#ifndef __GLOBAL.H__
#define __GLOBAL.H__
#include "common.h"
#include "mindsensors-lightsensorarray.h"
#include "Register.h"
#include "NXTServo-lib.c"
#define ARDUINO_ADDRESS 0x08
#define SERVO_CONTROLLER_ADDRESS 0xb0

typedef tSensors SensorPort;
typedef tMotor MotorPort;
typedef int ServoPort;

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

typedef struct sMindServo {
	ServoPort port;
	int close;
	int open;
} MindServo;

typedef struct sSpeedServo {
	ServoPort port;
} SpeedServo;

typedef struct sArduinoServo {
	ubyte address;
} ArduinoServo;

SensorPort arduino;
SensorPort MSLSA;
SensorPort touch;
SensorPort servoController;
MotorPort LMotor;
MotorPort RMotor;
MotorPort liftMotor;
LaserSensor leftDist;
LaserSensor frontDist;
LaserSensor rightDist;
PingSensor frontPing;
ColorSensor leftFrontL;
ColorSensor leftFrontM;
ColorSensor middleFront;
ColorSensor rightFrontR;
ColorSensor rightFrontM;
MindServo dumperServo;
MindServo clawServo;
ArduinoServo frontColorServo;
int forward = 30;
int turnForward = 50;
int turnBackward = -15;
const float wheelbase = 16.5;
const float width = 13.5;
const float length = 15;
const float diameter = 3;
const float cm = 360.0 / (diameter *  PI);
const int numOfIterations = 31;
const float obstacleThreshold = 8;
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

void generateServo(MindServo* serv, ServoPort port, int open, int close)
{
	serv->port = port;
	serv->open = open;
	serv->close = close;
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

void resetSync()
{
#ifdef NXT
	nSyncedMotors = synchNone;
#endif
}

// turning the robot right a certain amount of degrees
void turnRight(float deg, int power = forward)
{
	float wheelbaseEx = wheelbase;
	float target = wheelbaseEx*PI*(deg/360)*cm;
	resetEncoders();
	nMotorEncoderTarget[LMotor] = target;
	nSyncedMotors = synchAC;
	nSyncedTurnRatio = -100;
	motor[LMotor] = power;
	while(stillRunning(LMotor)){}
	resetSync();
	stopMotors();
	delay(100);
}

void turnLeft(float deg, int power = forward)
{
	float wheelbaseEx = wheelbase;
	float target = wheelbaseEx*PI*(deg/360)*cm;
	resetEncoders();
	nMotorEncoderTarget[LMotor] = -target;
	nSyncedMotors = synchAC;
	nSyncedTurnRatio = -100;
	motor[LMotor] = -power;
	while(stillRunning(LMotor)){}
	resetSync();
	stopMotors();
	delay(100);
}

void turnAbs(float deg, int power = forward)
{
	if(deg > 0)
		turnRight(abs(deg), power);
	else if (deg < 0)
		turnLeft(abs(deg), power);
}

void goStraight(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	nSyncedMotors = synchAC;
	nMotorEncoderTarget[LMotor] = dist;
	nSyncedTurnRatio = 100;
	motor[LMotor] = power;
	while(stillRunning(LMotor)){}
	resetSync();
	stopMotors();
}

void goBack(float dist, int power = forward)
{
	resetEncoders();
	dist *= cm;
	nSyncedMotors = synchAC;
	nMotorEncoderTarget[LMotor] = -dist;
	nSyncedTurnRatio = 100;
	motor[LMotor] = -power;
	while(stillRunning(LMotor)){}
	resetSync();
	stopMotors();
}

float getDistance(LaserSensor sensor)
{
	if (sensor < LEFT_LASER || sensor > RIGHT_LASER)
		return -1;
	int ret = 0;
	tByteArray send;
	tByteArray receive;
	do {
		send[0] = 2;
		send[1] = ARDUINO_ADDRESS;
		send[2] = sensor.address;
		writeI2C(arduino,send);
		wait1Msec(8);
		send[2] = 0x01;
		writeI2C(arduino,send,receive,2);
		ret = receive[1] << 8 | receive[0];
	} while (ret == 0);
	return (float)ret/10;
}

float getDistance(PingSensor sensor)
{
	if (sensor != FRONT_PING)
		return -1;
	int ret = 0;
	float cm;
	tByteArray send;
	tByteArray receive;
	do {
		send[0] = 2;
		send[1] = ARDUINO_ADDRESS;
		send[2] = sensor.address;
		writeI2C(arduino,send,receive,2);
		ret = receive[1] << 8 | receive[0];
		cm = (float)ret / 29 / 2;
		delay(10);
	} while (cm > 400 || cm == 0);
	return cm;
}

float getDistanceFiltered(PingSensor sensor)
{
	int iter = 10;
	float sum = 0;
	for (int i = 0; i < iter; i++)
	{
		sum += getDistance(sensor);
		delay(5);
	}
	sum /= iter;
	return sum;
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
	wait1Msec(3);
#ifdef DEBUG
	writeDebugStreamLine("Time End: %d", nPgmTime);
#endif
	send[2] = 0x01;
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
	if (sensor.address < LEFT_FRONT || sensor.address > RIGHT_FRONT)
			return cInvalid;
	Color curr;
	int red,green,blue;
	bool cont = false;
	do {
		cont = false;
		getColorRGB(sensor,red,green,blue);
		if (sensor.isLight) {
			//if (sensor.clear > 9000)
			//	curr = cSilver;
			//else
				curr = (Color) (sensor.clear < sensor.bwThreshold);
				break;
		}
		//if (sensor.clear > 8000)
		//{
		//	curr = cSilver;
		//	break;
		//}
		if (green < sensor.blackThreshold) {
			curr = cBlack;
			break;
		}
		if (green > sensor.whiteThreshold) {
			curr = cWhite;
			break;
		}
		if (blue == 0){
			cont = true;
			continue;
		}
		if ((float)green/blue > sensor.greenRatio) {
			curr = cGreen;
			break;
		}
		//writeDebugStreamLine("GRADIENT %d: %d %d", sensor.address, green, red);
		curr = cGradient;
	} while (cont);
	sensor.currentColor = curr;
	return curr;
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
	//if (middle == cSilver)
	//	middle = cWhite;
	//writeDebugStreamLine("SEELINE: %d %d %d %d %d", leftL, leftM, middle, rightM, rightR);
	return (leftL != cWhite || leftM != cWhite || middle != cWhite || rightM != cWhite || rightR != cWhite);
}

void openServo(MindServo serv)
{
	NXTServo_SetPosition(servoController, SERVO_CONTROLLER_ADDRESS, serv.port, serv.open);
}

void closeServo(MindServo serv)
{
	NXTServo_SetPosition(servoController, SERVO_CONTROLLER_ADDRESS, serv.port, serv.close);
}

void setPositionArduino(ArduinoServo serv, int position)
{
	tByteArray send;
	send[0] = 4;
	send[1] = ARDUINO_ADDRESS;
	send[2] = serv.address;
	send[3] = (position >> 8) & 0xFF;
	send[4] = (position) & 0xFF;
	writeI2C(arduino,send);
}

void setSpeedServo(SpeedServo serv)
{

}

void lowerClaw()
{

}

void raiseClaw()
{

}

void suspend()
{
	stopMotors();
	while(true){}
}

bool equals(float val1, float val2, float tolerance)
{
	return abs(val1 - val2) < tolerance;
}

int getLightValue()
{
	tByteArray send;
	tByteArray recieve;
	send[0] = 2;
	send[1] = 0x52;
	send[2] = 0x54;
	writeI2C(S1,send,recieve,1);
	return (int) recieve[0];
}

#endif

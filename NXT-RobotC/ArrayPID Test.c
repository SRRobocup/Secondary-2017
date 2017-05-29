#include "common.h"
#include  "Global.h"
#include "mindsensors-lightsensorarray.h"

void arrayPID()
{
	ubyte values[8];
	int upperLimit = 75;
	int lowerLimit = 30;
	float kp = 0.02;
	int tp = 30;
	float curr;
	MSLSAreadSensors(MSLSA,values);
	for (int i = 0; i < 8; i++) {
		writeDebugStream("%d ", values[i]);
	}
	curr = (upperLimit - clamp(values[0],lowerLimit,upperLimit)) * 500 +
	(upperLimit - clamp(values[1],lowerLimit,upperLimit)) * 100 +
	(upperLimit - clamp(values[2],lowerLimit,upperLimit)) * 20 +
	(upperLimit - clamp(values[3],lowerLimit,upperLimit)) * 1 +
	(upperLimit - clamp(values[4],lowerLimit,upperLimit)) * -1 +
	(upperLimit - clamp(values[5],lowerLimit,upperLimit)) * -20 +
	(upperLimit - clamp(values[6],lowerLimit,upperLimit)) * -100 +
	(upperLimit - clamp(values[7],lowerLimit,upperLimit)) * -500;
	motor[LMotor] = tp - curr*kp;
	motor[RMotor] = tp + curr*kp;
	writeDebugStreamLine("CURR: %f %d %d", curr, motor[LMotor], motor[RMotor]);
}

task main()
{
	ubyte values[8];
	LMotor = motorA;
	RMotor = motorC;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	MSLSA = S2;
	SensorType[MSLSA] = sensorI2CCustom;
	MSLSAinit(MSLSA);
	//MSLSAcalWhite(MSLSA);
	//displayTextLine(4, "Place on Black");
	//while(!getXbuttonValue(xButtonEnter)){}
	//wait1Msec(500);
	//MSLSAcalBlack(MSLSA);
	//delay(500);
	//MSLSASleep(MSLSA);
	//delay(500);
	//MSLSAinit(MSLSA);
	//displayTextLine(4, "Done Calib");
	//while(!getXbuttonValue(xButtonEnter)){}
	//wait1Msec(500);

	motor[LMotor] = 0;
	motor[RMotor] = 0;
	ubyte arr[8];
	while (true) {
		arrayPID();
		//MSLSAreadSensors(MSLSA,arr);
		//for (int i = 0; i < 8; i++)
		//	writeDebugStream("%d ", arr[i]);
		//writeDebugStreamLine("");
	}
}

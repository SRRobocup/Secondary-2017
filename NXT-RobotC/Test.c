#include "Global.h"

void setup()
{
	arduino = S1;
	MSLSA = S3;
	LMotor = motorA;
	RMotor = motorC;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	SensorType[arduino] = sensorI2CCustom9V;
	SensorType[MSLSA] = sensorI2CCustom9V;
	leftDist.address = LEFT_LASER;
	frontDist.address = FRONT_LASER;
	rightDist.address = RIGHT_LASER;
	frontPing.address = FRONT_PING;
	MSLSAinit(MSLSA);
	float values[25] = {6728.5,12072.0,711.7,2966.33,0.8,6041.0,11019.0,703.7,2429.67,0.7,12027.5,21499.0,835.0,8615.33,0.9,5020.5,9512.0,712.3,2415.00,0.8,4530.0,8231.0,694.0,1827.67, 0.8};
	generateColor(&leftFrontL,LEFT_FRONT,values[0],values[1],values[2],values[3],values[4],true);
	generateColor(&leftFrontM,LEFT_MIDDLE,values[5],values[6],values[7],values[8],values[9],true);
	generateColor(&middleFront,MIDDLE_FRONT,values[10],values[11],values[12],values[13],values[14],true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,values[15],values[16],values[17],values[18],values[19],true);
	generateColor(&rightFrontR,RIGHT_FRONT,values[20],values[21],values[22],values[23],values[24],true);
}

const float kP = 8
const float kI = 0;
const float kD = 0;
float P, I, D, lastP;
float threshold = 4.5;
float adjust;
float tp = 20;

task main()
{
	setup();
	int r,g,b,c;
	while (true)
	{
		P = getDistance(rightDist) - threshold;
		writeDebugStreamLine("P: %f", P);
		I += P;
		if (abs(P) < 1.5)
			I = 0;
		D = P - lastP;
		adjust = P*kP + I*kI + D*kD;
		adjust = 0;
		motor[LMotor] = tp + adjust;
		motor[RMotor] = tp - adjust;
		//delay(1);
	//writeDebugStreamLine("DIST: %f", getDistance(rightDist));
		//getColorRGB(middleFront,r,g,b,c);
		//writeDebugStreamLine("COLOR: %d",c);
		//delay(500);
	}
}

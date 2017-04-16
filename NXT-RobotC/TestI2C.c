//#include "common.h"
//#define DEBUG_COMMON_H
//#define DEBUG
//#define SHORT_DEBUG
#include "Global.h"

void setup()
{
	arduino = S1;
	MSLSA = S3;
	LMotor = motorA;
	RMotor = motorC;
	bMotorReflected[motorA] = true;
	bMotorReflected[motorC] = true;
	SensorType[arduino] = sensorI2CCustom9V;
	SensorType[MSLSA] = sensorI2CCustom9V;
	leftDist.address = LEFT_LASER;
	frontDist.address = FRONT_LASER;
	rightDist.address = RIGHT_LASER;
	frontPing.address = FRONT_PING;
	MSLSAinit(MSLSA);
	float values[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	generateColor(&leftFrontL,LEFT_FRONT,6602,100000,values[2],values[3],values[4],true);
	generateColor(&leftFrontM,LEFT_MIDDLE,5894,100000,values[7],values[8],values[9],true);
	generateColor(&middleFront,MIDDLE_FRONT,11659.5,100000,values[12],values[13],values[14],true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,4686,100000,values[17],values[18],values[19],true);
	generateColor(&rightFrontR,RIGHT_FRONT,4112,100000,values[22],values[23],values[24],true);
}

task main()
{
	setup();
	clearDebugStream();
	int r,g,b;
	while (true) {
		//writeDebugStreamLine("LEFT LASER: %f",getDistance(leftDist));
		//displayTextLine(0,"LEFT: %f",getDistance(leftDist));
		//writeDebugStreamLine("MIDDLE: %f",getDistance(frontDist));
		//displayTextLine(1,"MIDDLE: %f",getDistance(frontDist));
		//writeDebugStreamLine("RIGHT LASER: %f",getDistance(rightDist));
		//displayTextLine(2,"RIGHT: %f",getDistance(rightDist));
		//delay(5);
		getColorRGB(leftFrontL,r,g,b);
		writeDebugStreamLine("LF COLOR %x: %d %d %d %d",LEFT_FRONT,r,g,b,leftFrontL.clear);
		displayTextLine(3,"LF: %d %d %d %d",r,g,b,leftFrontL.clear);
		//delay(5);
		getColorRGB(leftFrontM,r,g,b);
		writeDebugStreamLine("LM COLOR %x: %d %d %d %d",LEFT_MIDDLE,r,g,b,leftFrontM.clear);
		displayTextLine(4,"LM: %d %d %d %d",r,g,b,leftFrontM.clear);
		//delay(5);
		getColorRGB(middleFront,r,g,b);
		writeDebugStreamLine("MF COLOR %x: %d %d %d %d",MIDDLE_FRONT,r,g,b,middleFront.clear);
		displayTextLine(5,"MF: %d %d %d %d",r,g,b,middleFront.clear);
		//delay(5);
		getColorRGB(rightFrontM,r,g,b);
		writeDebugStreamLine("RM COLOR %x: %d %d %d %d",RIGHT_MIDDLE,r,g,b,rightFrontM.clear);
		displayTextLine(6,"RM: %d %d %d %d",r,g,b,rightFrontM.clear);
		//delay(5);
		getColorRGB(rightFrontR,r,g,b);
		writeDebugStreamLine("RF COLOR %x: %d %d %d %d",RIGHT_FRONT,r,g,b,rightFrontR.clear);
		displayTextLine(7,"RF: %d %d %d %d",r,g,b,rightFrontR.clear);

		writeDebugStreamLine("------------------------------------------------------------------------------");
		//delay(750);
	}
}

//#include "common.h"
//#define DEBUG_COMMON_H
//#define DEBUG
//#define SHORT_DEBUG
#include "Global.h"

void setup()
{
	arduino = S3;
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
	// 									BW			Sil		Black	White  Rat  BW		 Sil		Black	White  Rat  BW      Sil     Black White  Rat  BW     Sil    Black White  Rat   BW    Sil    Black White  Rat
	float values[25] = {3779.0,3393.0,726.7,1683.0,1.38,3936.0,4734.0,739.3,1566.3,1.31,3252.0,2777.0,741.3,1326.0,1.38,4108.0,3404.0,723.3,1694.0,1.20,3106.0,2623.0,699.3,1320.0,1.42};
	generateColor(&leftFrontL,LEFT_FRONT,values[0],values[1],values[2],values[3],values[4],true);
	generateColor(&leftFrontM,LEFT_MIDDLE,values[5],values[6],values[7],values[8],values[9],false);
	generateColor(&middleFront,MIDDLE_FRONT,values[10],values[11],values[12],values[13],values[14],true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,values[15],values[16],values[17],values[18],values[19],false);
	generateColor(&rightFrontR,RIGHT_FRONT,values[20],values[21],values[22],values[23],values[24],true);
}

task main()
{
	setup();
	clearDebugStream();
	int r,g,b;
	while (true) {
		writeDebugStreamLine("LEFT LASER: %f",getDistance(leftDist));
		displayTextLine(0,"LEFT: %f",getDistance(leftDist));
		//writeDebugStreamLine("MIDDLE: %f",getDistance(frontDist));
		//displayTextLine(1,"MIDDLE: %f",getDistance(frontDist));
		writeDebugStreamLine("RIGHT LASER: %f",getDistance(rightDist));
		displayTextLine(2,"RIGHT: %f",getDistance(rightDist));
		writeDebugStreamLine("FRONT PING: %f",getDistance(frontPing));
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
		//delay(500);
	}
}

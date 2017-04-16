#include "Global.h"
#include "LineTrace.h"
#include "Obstacle.h"
#include "Evac.h"

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
	clearDebugStream();
	setup();
	while (true)
	{
		getColor(leftFrontL);
		getColor(leftFrontM);
		getColor(middleFront);
		getColor(rightFrontM);
		getColor(rightFrontR);
		//writeDebugStreamLine("%d %d %d %d %d", leftFrontL.currentColor, leftFrontM.currentColor, middleFront.currentColor, rightFrontM.currentColor, rightFrontR.currentColor);
		//writeDebugStreamLine("CLEAR: %d %d %d %d %d", leftFrontL.clear, leftFrontM.clear, middleFront.clear, rightFrontM.clear, rightFrontR.clear);
		writeDebugStreamLine("GREEN: %d %d %d %d %d", leftFrontL.green, leftFrontM.green, middleFront.green, rightFrontM.green, rightFrontR.green);

		//if (getDistance(frontPing) < obstacleThreshold)
		//{
			//obstacle();
		//}
		lineTrace();
	}
	evac();
}

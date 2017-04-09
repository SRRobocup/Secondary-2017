#include "Global.h"
#include "LineTrace.h"
#include "Obstacle.h"
#include "Evac.h"

void setup()
{
	arduino = S1;
	MSLSA = S3;
	LMotor = motorC;
	RMotor = motorA;
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
	float values[25] = {7331.0,5618.0,716.3,3335.0,1.59,6464.5,5088.0,729.7,2944.0,1.37,5788.0,6389.0,728.7,3011.7,1.49,5032.0,4075.0,724.0,2422.0,1.41,3891.0,3664.0,690.7,1811.3,1.28};
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
		leftFrontL.currentColor = getColor(leftFrontL);
		leftFrontM.currentColor = getColor(leftFrontM);
		middleFront.currentColor = getColor(middleFront);
		rightFrontM.currentColor = getColor(rightFrontM);
		rightFrontR.currentColor = getColor(rightFrontR);
		//writeDebugStreamLine("%d %d %d %d %d", leftFrontL.currentColor, leftFrontM.currentColor, middleFront.currentColor, rightFrontM.currentColor, rightFrontR.currentColor);
		//writeDebugStreamLine("CLEAR: %d %d %d %d %d", leftFrontL.clear, leftFrontM.clear, middleFront.clear, rightFrontM.clear, rightFrontR.clear);
		writeDebugStreamLine("GREEN: %d %d %d %d %d", leftFrontL.green, leftFrontM.green, middleFront.green, rightFrontM.green, rightFrontR.green);

		//if (getDistance(frontPing) < obstacleThreshold)
		//{
		//	obstacle();
		//}
		lineTrace();
	}
	evac();
}

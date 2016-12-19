#include "Global.h"
#include "LineTrace.h"
#include "Obstacle.h"
#include "Evac.h"

void setup()
{
	LMotor = motorA;
	RMotor = motorC;
	bMotorReflected[motorA] = true;
	bMotorReflected[motorC] = true;
	SensorType[arduino] = sensorI2CCustomFast9V;
	leftDist.address = 0x42;
	frontDist.address = 0x43;
	rightDist.address = 0x44;
	downLeftDist.address = 0x45;
	downRightDist.address = 0x46;
	frontPing.address = 0x47;
	float values[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	generateColor(&leftFrontL,0x48,values[0],values[1],values[2],values[3],values[4],true);
	generateColor(&leftFrontM,0x49,values[5],values[6],values[7],values[8],values[9],false);
	generateColor(&middleFront,0x50,values[10],values[11],values[12],values[13],values[14],true);
	generateColor(&rightFrontM,0x51,values[15],values[16],values[17],values[18],values[19],false);
	generateColor(&rightFrontR,0x52,values[20],values[21],values[22],values[23],values[24],true);
}

task main()
{
	setup();
	while (middleFront.currentColor != cSilver)
	{
		leftFrontL.currentColor = getColor(leftFrontL);
		leftFrontM.currentColor = getColor(leftFrontM);
		middleFront.currentColor = getColor(middleFront);
		rightFrontM.currentColor = getColor(rightFrontM);
		rightFrontR.currentColor = getColor(rightFrontR);

		if (getDistance(frontPing) < obstacleThreshold)
		{
			obstacle();
		}
		lineTrace();
	}
	evac();
}

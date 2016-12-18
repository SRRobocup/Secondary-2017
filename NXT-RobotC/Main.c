#include "Global.h"
#include "LineTrace.h"
#include "Obstacle.h"
#include "Evac.h"

void setup()
{
	leftDist.address = 0x42;
	frontDist.address = 0x43;
	rightDist.address = 0x44;
	downLeftDist.address = 0x45;
	downRightDist.address = 0x46;
	frontPing.address = 0x47;
	float values[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	generateColor(&leftFrontL,0x48,values[0],values[1],values[2],false);
	generateColor(&leftFrontM,0x49,values[3],values[4],values[5],false);
	generateColor(&middleFront,0x50,values[6],values[7],values[8],true);
	generateColor(&rightFrontM,0x51,values[9],values[10],values[11],false);
	generateColor(&rightFrontR,0x52,values[12],values[13],values[14],false);
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

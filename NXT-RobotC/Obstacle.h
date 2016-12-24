#include "Global.h"

void obstacle()
{
	const float kP = 1.5;
	const float kI = 1.5;
	const float kD = 1.5;
	float P, I, D, lastP;
	float threshold = 0;
	float dir; //1 is left, -1 is right
	float lFirst = getDistance(leftDist);
	float rFirst = getDistance(rightDist);
	float lDist = clamp(lFirst,0,30);
	float rDist = clamp(rFirst,0,30);
	float adjust;
	float tp = 20;
	dir = lDist > rDist ? 1 : -1; //bias is set here
	if (dir == 1)
	{
		turnLeft(90);
		middleFront.isLight = true;
		while (getColor(middleFront) != cBlack)
		{
			P = getDistance(rightDist) - threshold;
			I += P;
			if (abs(P) < 1.5)
				I = 0;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			motor[LMotor] = tp + adjust;
			motor[RMotor] = tp - adjust;
		}
		goStraight(5);
		motor[LMotor] = -tp;
		motor[RMotor] = tp;
		while (getColor(middleFront) == cBlack){}
		while (getColor(middleFront) != cBlack){}
	}
	else
	{
		turnRight(90);
		middleFront.isLight = true;
		while (getColor(middleFront) != cBlack)
		{
			P = getDistance(leftDist) - threshold;
			I += P;
			if (abs(P) < 1.5)
				I = 0;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			motor[LMotor] = tp + adjust;
			motor[RMotor] = tp - adjust;
		}
		goStraight(5);
		motor[LMotor] = tp;
		motor[RMotor] = -tp;
		while (getColor(middleFront) == cBlack){}
		while (getColor(middleFront) != cBlack){}
	}
}

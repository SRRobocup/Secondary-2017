#include "Global.h"

void obstacle()
{
	const float kP = 8;
	const float kI = 0;
	const float kD = 0;
	float P, I, D, lastP;
	float threshold = 4.5;
	float dir; //1 is left, -1 is right
	float lFirst = getDistance(leftDist);
	float rFirst = getDistance(rightDist);
	float lDist = clamp(lFirst,0,30);
	float rDist = clamp(rFirst,0,30);
	float adjust;
	float tp = 20;
	dir = lDist > rDist ? 1 : -1; //bias is set here
	dir = -1;
	//dir = rDist > lDist ? -1 : 1;
	bool lastStateSensor = middleFront.isLight;
	middleFront.isLight = true;
	if (dir == 1)
	{
		turnLeft(70);
		while (getColor(middleFront) != cBlack)
		{
			P = getDistance(rightDist) - threshold;
			writeDebugStreamLine("P: %f", P);
			I += P;
			if (abs(P) < 1.5)
				I = 0;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			motor[LMotor] = tp - adjust;
			motor[RMotor] = tp + adjust;
			delay(8);
		}
		stopMotors();
		writeDebugStreamLine("MIDDLE FRONT: %d", getColor(middleFront));
		goStraight(5);
		resetEncoders();
		motor[LMotor] = -tp;
		motor[RMotor] = tp;
		while (getColor(middleFront) == cBlack){}
		while (getColor(middleFront) != cBlack && abs(nMotorEncoder[LMotor]) < wheelbase*PI/4*cm){}
		if (abs(nMotorEncoder[LMotor]) < wheelbase*PI/4*cm)
		{
			motor[LMotor] = tp;
			motor[RMotor] = -tp;
			while (getColor(middleFront) != cBlack){}
		}
	}
	else
	{
		turnRight(70);
		while (getColor(middleFront) != cBlack)
		{
			P = getDistance(leftDist) - threshold;
			writeDebugStreamLine("P: %f", P);
			I += P;
			if (abs(P) < 1.5)
				I = 0;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			motor[LMotor] = tp - adjust;
			motor[RMotor] = tp + adjust;
			delay(8);
		}
		goStraight(5);
		resetEncoders();
		motor[LMotor] = tp;
		motor[RMotor] = -tp;
		while (getColor(middleFront) == cBlack){}
		while (getColor(middleFront) != cBlack && abs(nMotorEncoder[LMotor]) < wheelbase*PI/4*cm){}
		if (abs(nMotorEncoder[LMotor]) < wheelbase*PI/4*cm)
		{
			motor[LMotor] = tp;
			motor[RMotor] = -tp;
			while (getColor(middleFront) != cBlack){}
		}
	}
	middleFront.isLight = lastStateSensor;
}

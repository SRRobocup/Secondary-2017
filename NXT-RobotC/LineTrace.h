#include "Global.h"

const int LEFTLBLACK = 10000;
const int LEFTMBLACK = 1000;
const int MIDDLEBLACK = 100;
const int RIGHTMBLACK = 10;
const int RIGHTRBLACK = 1;
const int LEFTGREEN = 2000;
const int RIGHTGREEN = 20;
const int NONE = 0;

unsigned int getState()
{
	return (int) leftFrontL.currentColor*10000 + leftFrontM.currentColor*1000 + middleFront.currentColor*100 + rightFrontM.currentColor*10 + rightFrontR.currentColor;
}

void arrayPID()
{
	ubyte values[8];
	int upperLimit = 75;
	int lowerLimit = 30;
	float kp = 0;
	int tp = 30;
	int curr;
	MSLSAreadSensors(splitter,values);
	curr = (upperLimit - clamp(values[0],lowerLimit,upperLimit)) * 500 +
	(upperLimit - clamp(values[1],lowerLimit,upperLimit)) * 100 +
	(upperLimit - clamp(values[2],lowerLimit,upperLimit)) * 10 +
	(upperLimit - clamp(values[3],lowerLimit,upperLimit)) * 1 +
	(upperLimit - clamp(values[4],lowerLimit,upperLimit)) * -1 +
	(upperLimit - clamp(values[5],lowerLimit,upperLimit)) * -10 +
	(upperLimit - clamp(values[6],lowerLimit,upperLimit)) * -100 +
	(upperLimit - clamp(values[7],lowerLimit,upperLimit)) * -500;
	motor[motorA] = tp + curr*kp;
	motor[motorC] = tp - curr*kp;
}

void lineTrace()
{
	const float kP, kI, kD;
	float P,I = 0,D, lastP;
	float adjust;
	unsigned int state = getState();
	state = leftFrontM.currentColor == cGreen ? LEFTGREEN : state;
	state = rightFrontM.currentColor == cGreen ? RIGHTGREEN : state;
	if (state == NONE && seeBlackArray())
	{
		state = 00100;
	}
	switch (state)
	{
		case NONE:
			displayTextLine(0,"State: GAP");
			goBack(7);
			for(int i = 0; i < 5; i++)
				while(seeBlackArray())
					arrayPID();
			while (!seeLine())
			{
				motor[LMotor] = forward;
				motor[RMotor] = forward;
			}
			stopMotors();
			break;
		case LEFTGREEN:
			displayTextLine(0,"State: LEFTGREEN");
			goStraight(7);
			nMotorEncoder[RMotor] = 0;
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while(getColor(middleFront) == cBlack){}
			while(getColor(middleFront) != cBlack && nMotorEncoder[RMotor] < wheelbase*PI/4){}
			stopMotors();
			break;
		case RIGHTGREEN:
			displayTextLine(0,"State: RIGHTGREEN");
			goStraight(7);
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while(getColor(middleFront) == cBlack){}
			while(getColor(middleFront) != cBlack && nMotorEncoder[LMotor] < wheelbase*PI/4){}
			stopMotors();
			stopMotors();
			break;
		case LEFTMBLACK:
			displayTextLine(0,"State: LEFT TURN");
			motor[LMotor] = turnBackward;
			motor[RMotor] = turnForward;
			break;
		case RIGHTMBLACK:
			displayTextLine(0,"State: RIGHT TURN");
			motor[LMotor] = turnForward;
			motor[RMotor] = turnBackward;
			break;
		case LEFTMBLACK + LEFTLBLACK:
			displayTextLine(0,"State: LEFT 90 TURN");
			goStraight(5);
			turnLeft(90);
			break;
		case RIGHTMBLACK + RIGHTRBLACK:
			displayTextLine(0,"State: RIGHT 90 TURN");
			goStraight(5);
			turnRight(90);
			break;
		default:
			displayTextLine(0,"State: STRAIGHT");
			//twoEye PID??
			//motor[LMotor] = forward;
			//motor[RMotor] = forward;
			P = leftFrontM.currentColor - rightFrontM.currentColor;
			I += P;
			if (abs(P) < 1.5)
				I = 0;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			motor[LMotor] = forward + adjust;
			motor[RMotor] = forward - adjust;
			break;
	}
}

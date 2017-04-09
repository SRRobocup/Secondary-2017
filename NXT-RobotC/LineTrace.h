#include "Global.h"

const int LEFTLBLACK = 10000;
const int LEFTMBLACK = 1000;
const int MIDDLEBLACK = 100;
const int RIGHTMBLACK = 10;
const int RIGHTRBLACK = 1;
const int LEFTGREEN = 2000;
const int RIGHTGREEN = 20;
const int NONE = 0;

unsigned int lastState = 00100;

unsigned int getState()
{
	return (int) leftFrontL.currentColor*10000 + leftFrontM.currentColor*1000 + middleFront.currentColor*100 + rightFrontM.currentColor*10 + rightFrontR.currentColor;
}

void arrayPID()
{
	ubyte values[8];
	int upperLimit = 75;
	int lowerLimit = 30;
	float kp = 0.02;
	int tp = 30;
	int curr;
	MSLSAreadSensors(MSLSA,values);
	curr = (upperLimit - clamp(values[0],lowerLimit,upperLimit)) * 500 +
	(upperLimit - clamp(values[1],lowerLimit,upperLimit)) * 100 +
	(upperLimit - clamp(values[2],lowerLimit,upperLimit)) * 20 +
	(upperLimit - clamp(values[3],lowerLimit,upperLimit)) * 1 +
	(upperLimit - clamp(values[4],lowerLimit,upperLimit)) * -1 +
	(upperLimit - clamp(values[5],lowerLimit,upperLimit)) * -20 +
	(upperLimit - clamp(values[6],lowerLimit,upperLimit)) * -100 +
	(upperLimit - clamp(values[7],lowerLimit,upperLimit)) * -500;
	motor[motorA] = tp + curr*kp;
	motor[motorC] = tp - curr*kp;
}

void lineTrace()
{
	const float kP = 0.002, kI = 0, kD = 0;
	float P,I = 0,D, lastP;
	float adjust;
	unsigned int state = getState();
	state = leftFrontM.currentColor == cGreen ? LEFTGREEN : state;
	state = rightFrontM.currentColor == cGreen ? RIGHTGREEN : state;
	if (state == NONE && seeBlackArray())
	{
		state = 00100;
	}
	writeDebugStream("STATE: %d ", state);
	switch (state)
	{
		case NONE:
			displayTextLine(0,"State: GAP");
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while (nMotorEncoder[LMotor] < 5*cm)
			{
				if (seeLine() || seeBlackArray())
					return;
			}
			while (!seeBlackArray())
			{
				motor[LMotor] = -30;
				motor[RMotor] = -30;
			}
			goBack(5);
			int startTime = 0, timeLimit = 1500;
			for(int i = 0; i < 4; i++)
			{
				startTime = nPgmTime;
				while(seeBlackArray())
				{
					if (nPgmTime - startTime > timeLimit)
						return;
					arrayPID();
				}
				goBack(5);
			}
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while (seeLine()){}
			while (!seeLine()) {}
			stopMotors();
			break;
		case LEFTGREEN + RIGHTGREEN:
			displayTextLine(0, "State: DEADEND");
			writeDebugStreamLine("DEAD END");
			suspend();
			break;
		case LEFTGREEN:
			displayTextLine(0,"State: LEFTGREEN");
			writeDebugStreamLine("LEFTGREEN");
			while (getColor(leftFrontM) == cGradient || leftFrontM.currentColor == cGradient){}
			if (leftFrontM.currentColor == cWhite)
				return;
			goStraight(7);
			nMotorEncoder[RMotor] = 0;
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[RMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(middleFront) != cWhite){}
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while (getColor(middleFront) == cWhite){}
			//suspend();
			stopMotors();
			break;
		case RIGHTGREEN:
			displayTextLine(0,"State: RIGHTGREEN");
			writeDebugStreamLine("RIGHTGREEN")
			while (getColor(rightFrontM) == cGradient || rightFrontM.currentColor == cGradient){}
			if (rightFrontM.currentColor == cWhite)
				return;
			goStraight(7);
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[LMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(middleFront) != cWhite){}
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while (getColor(middleFront) == cWhite){}
			//suspend();
			stopMotors();
			break;
		case LEFTLBLACK:
		//case LEFTMBLACK + LEFTLBLACK:
		case LEFTMBLACK:
			displayTextLine(0,"State: LEFT TURN");
			writeDebugStreamLine("LEFT TURN");
			motor[LMotor] = turnBackward;
			motor[RMotor] = turnForward;
			break;
		case RIGHTRBLACK:
		//case RIGHTMBLACK + RIGHTRBLACK:
		case RIGHTMBLACK:
			displayTextLine(0,"State: RIGHT TURN");
			writeDebugStreamLine("RIGHT TURN");
			motor[LMotor] = turnForward;
			motor[RMotor] = turnBackward;
			break;
		case LEFTMBLACK + LEFTLBLACK:
			displayTextLine(0,"State: LEFT 90 TURN");
			writeDebugStreamLine("LEFT 90");
			goStraight(7);
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			nMotorEncoder[RMotor] = 0;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[RMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(middleFront) != cWhite){}
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while (getColor(middleFront) == cWhite){}
			stopMotors();
			break;
		case RIGHTMBLACK + RIGHTRBLACK:
			displayTextLine(0,"State: RIGHT 90 TURN")
			writeDebugStreamLine("RIGHT 90");
			goStraight(7);
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			nMotorEncoder[LMotor] = 0;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[LMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(middleFront) != cWhite){}
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while (getColor(middleFront) == cWhite){}
			stopMotors();
			break;
		case MIDDLEBLACK + LEFTMBLACK + LEFTMBLACK:
		case MIDDLEBLACK + RIGHTMBLACK + RIGHTRBLACK:
		case LEFTLBLACK + LEFTMBLACK + MIDDLEBLACK + RIGHTMBLACK + RIGHTRBLACK:
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			break;
		default:
			displayTextLine(0,"State: STRAIGHT");
			writeDebugStreamLine("STRAIGHT"):
			//twoEye PID??
			//motor[LMotor] = forward;
			//motor[RMotor] = forward;
			P = leftFrontM.clear - rightFrontM.clear;
			writeDebugStreamLine("%d", P);
			if (abs(P) < 800)
				I = 0;
			else
				I -= P;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			//adjust = 0;
			motor[LMotor] = forward + adjust;
			motor[RMotor] = forward - adjust;
			break;
	}
	lastState = state;
}

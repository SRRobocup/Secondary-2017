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
unsigned int lastTurn  = 00100;

unsigned int getState()
{
	return (int) leftFrontL.currentColor*10000 +
	            leftFrontM.currentColor*1000 +
	            middleFront.currentColor*100 +
	            rightFrontM.currentColor*10 +
	            rightFrontR.currentColor;
}

void arrayPID()
{
	ubyte values[8];
	int upperLimit = 75;
	int lowerLimit = 30;
	float kp = 0.02;
	float forwardDistance = 7;
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
	motor[LMotor] = tp - curr*kp;
	motor[RMotor] = tp + curr*kp;
}

void lineTrace()
{
	//0.002, 0.001, 0.0015
	const float kP = 0.001, kI = 0.000, kD = 0.000;
	float P,I = 0,D, lastP;
	float adjust;
	float forwardDistance = 7;
	unsigned int state = getState();
	writeDebugStreamLine("UNCHANGED STATE: %d",state);
	if (!(leftFrontM.currentColor == cGreen && rightFrontM.currentColor == cGreen))
	{
		//stopMotors();
		if (leftFrontL.currentColor == cGreen)
		{
			state = LEFTGREEN;
		}
		else if (rightFrontR.currentColor == cGreen)
		{
			state = RIGHTGREEN;
		}
		else if (leftFrontM.currentColor == cGreen)
		{
			if (getColor(rightFrontM) == cGreen || getColor(rightFrontM) == cGreen)
				state = 02020;
			else
			if (leftFrontL.currentColor == cBlack)
				state = RIGHTGREEN;
			else
				state = LEFTGREEN;
		}
		else if (rightFrontM.currentColor == cGreen)
		{
			if (getColor(leftFrontM) == cGreen || getColor(leftFrontM) == cGreen)
				state = 02020;
			else
			if (rightFrontR.currentColor == cBlack)
				state = LEFTGREEN;
			else
				state = RIGHTGREEN;
		}
		switch (state)
		{
			case 10001:
			case 30003:
			case 3030:
			case 1030:
				state = lastTurn;
				break;
			case 33:
			case 30:
			case 3:
				state = RIGHTMBLACK;
				break;
		}
	}
	else
		state = 02020;
	if (state == NONE && lastState == (LEFTLBLACK + LEFTMBLACK) && lastState == (RIGHTRBLACK + RIGHTMBLACK))
 	{
 		int dir = lastState == (LEFTLBLACK + LEFTMBLACK) ? 1 : -1;
 		motor[LMotor] = forward*dir;
 		motor[RMotor] = forward*-dir;
 		while (getColor(middleFront) == cWhite){}
 		stopMotors();
 	}
	if (state == NONE && seeBlackArray())
	{
		state = 00100;
	}
	writeDebugStream("STATE: %5d ", state);
	if (state == 13000)
		state = 11000;
	if (state == 31)
		state = 11;
	switch (state)
	{
		case NONE:
			displayTextLine(0,"State: GAP");
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while (nMotorEncoder[LMotor] < 5*cm)
			{
				if (seeLine())
				{
					writeDebugStreamLine("FOUND LINE");
					return;
				}
			}
			while (!seeBlackArray())
			{
				motor[LMotor] = -30;
				motor[RMotor] = -30;
			}
			stopMotors();
			//goBack(5);
			goBack(5);
			int startTime = 0, timeLimit = 2000;
			for(int i = 0; i < 4; i++)
			{
				startTime = nPgmTime;
				while(seeBlackArray())
				{
					if (nPgmTime - startTime > timeLimit)
					{
						writeDebugStreamLine("FOUND LINE");
						return;
					}
					arrayPID();
				}
				writeDebugStreamLine("ITERATION %d DONE",i);
				goBack(4);
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
			playSound(soundBeepBeep);
			//suspend();
			goStraight(forwardDistance);
			turnRight(180);
			break;
		case LEFTGREEN:
			displayTextLine(0,"State: LEFTGREEN");
			writeDebugStreamLine("LEFTGREEN");
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while (leftFrontM == cGreen || leftFrontM.currentColor == cGradient){ leftFrontM.currentColor = getColor(leftFrontM); }
			if (leftFrontM.currentColor == cWhite)
				return;
			lastTurn = RIGHTMBLACK;
			goStraight(forwardDistance);
			turnLeft(10);
			nMotorEncoder[RMotor] = 0;
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[RMotor] < wheelbase*PI*2/9*cm){}
			if (nMotorEncoder[RMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(leftFrontL) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			//while (getColor(middleFront) != cWhite){}
			//motor[LMotor] = forward;
			//motor[RMotor] = -forward;
			//while (getColor(middleFront) == cWhite){}
			//while ((getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack) && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
			//suspend();
			stopMotors();
			break;
		case RIGHTGREEN:
			displayTextLine(0,"State: RIGHTGREEN");
			writeDebugStreamLine("RIGHTGREEN");
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while (rightFrontM.currentColor == cGreen || rightFrontM.currentColor == cGradient){ rightFrontM.currentColor = getColor(rightFrontM); }
			if (rightFrontM.currentColor == cWhite)
				return;
			lastTurn = LEFTMBLACK;
			goStraight(forwardDistance);
			turnRight(10);
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while (getColor(middleFront) != cWhite){}
			while (getColor(middleFront) == cWhite && nMotorEncoder[LMotor] < wheelbase*PI*2/9*cm){}
			if (nMotorEncoder[LMotor] >= wheelbase*PI/4*cm)
				return;
			while (getColor(rightFrontR) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			//while (getColor(middleFront) != cWhite){}
			//motor[LMotor] = -forward;
			//motor[RMotor] = forward;
			//while (getColor(middleFront) == cWhite){}
			//while ((getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack) && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
			//suspend();
			stopMotors();
			break;
		case LEFTLBLACK:
		//case LEFTMBLACK + LEFTLBLACK:
		case LEFTMBLACK:
			displayTextLine(0,"State: LEFT TURN");
			writeDebugStreamLine("LEFT TURN");
			//lastTurn = LEFTMBLACK;
			motor[LMotor] = turnBackward;
			motor[RMotor] = turnForward;
			break;
		case RIGHTRBLACK:
		//case RIGHTMBLACK + RIGHTRBLACK:
		case RIGHTMBLACK:
			displayTextLine(0,"State: RIGHT TURN");
			writeDebugStreamLine("RIGHT TURN");
			//lastTurn = RIGHTMBLACK;
			motor[LMotor] = turnForward;
			motor[RMotor] = turnBackward;
			break;
		case LEFTMBLACK + LEFTLBLACK:
			displayTextLine(0,"State: LEFT 90 TURN");
			writeDebugStreamLine("LEFT 90");
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			nMotorEncoder[LMotor] = 0;
			while (nMotorEncoder[LMotor] < forwardDistance*cm)
			{
				if (getColor(rightFrontM) == cBlack)
					return;
			}
			//lastTurn = LEFTMBLACK;
			turnLeft(90);
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			nMotorEncoder[LMotor] = 0;
			while (getColor(middleFront) != cWhite){}
			while (!seeLine() && nMotorEncoder[LMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[LMotor] < wheelbase*PI/4*cm)
			{
				//while (getColor(middleFront) != cWhite){}
				//motor[LMotor] = forward;
				//motor[RMotor] = -forward;
				//while (getColor(middleFront) == cWhite){}
				//motor[LMotor] = -forward;
				//motor[RMotor] = forward;
				while (getColor(rightFrontR) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
				if (getColor(leftFrontL) == cBlack)
					while (getColor(middleFront) == cWhite){}
				motor[LMotor] = -forward;
				motor[RMotor] = -forward;
				while ((getColor(leftFrontM) != cBlack || getColor(rightFrontM) != cBlack) && abs(nMotorEncoder[LMotor]) < 5*cm){}
				if (abs(nMotorEncoder[LMotor]) >= 5*cm)
					while (getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
				if (!(getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen))
					goStraight(abs(nMotorEncoder[LMotor])/cm);
			}
			else if (!seeLine())
			{
				while (!seeLine() && nMotorEncoder[LMotor] < wheelbase*PI/2*cm){}
				if(nMotorEncoder[LMotor] >= wheelbase*PI/2*cm)
				{
					stopMotors();
					turnLeft(180);
					return;
				}
				while (getColor(rightFrontR) == cBlack || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			}
			stopMotors();
			//goBack(1);
			break;
		case RIGHTMBLACK + RIGHTRBLACK:
			displayTextLine(0,"State: RIGHT 90 TURN");
			writeDebugStreamLine("RIGHT 90");
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			nMotorEncoder[LMotor] = 0;
			while (nMotorEncoder[LMotor] < forwardDistance*cm)
			{
				if (getColor(leftFrontM) == cBlack)
					return;
			}
			//lastTurn = RIGHTMBLACK;
			turnRight(90);
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			nMotorEncoder[RMotor] = 0;
			while (getColor(middleFront) != cWhite){}
			while (!seeLine() && nMotorEncoder[RMotor] < wheelbase*PI/4*cm){}
			if (nMotorEncoder[RMotor] < wheelbase*PI/4*cm)
			{
				//while (getColor(middleFront) != cWhite){}
				//motor[LMotor] = -forward;
				//motor[RMotor] = forward;
				//while (getColor(middleFront) == cWhite){}
				//motor[LMotor] = forward;
				//motor[RMotor] = -forward;
				while (getColor(leftFrontL) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
				if (getColor(rightFrontR) == cBlack)
					while (getColor(middleFront) == cWhite){}
				motor[LMotor] = -forward;
				motor[RMotor] = -forward;
				while ((getColor(leftFrontM) != cBlack || getColor(rightFrontM) != cBlack) && abs(nMotorEncoder[LMotor]) < 5*cm){}
				if (abs(nMotorEncoder[LMotor]) >= 5*cm)
					while (getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
				if (!(getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen))
					goStraight(abs(nMotorEncoder[LMotor])/cm);
			}
			else
			{
				while (!seeLine() && nMotorEncoder[RMotor] < wheelbase*PI/2*cm){}
				if(nMotorEncoder[RMotor] >= wheelbase*PI/2*cm)
				{
					stopMotors();
					turnRight(180);
					return;
				}
				while (getColor(leftFrontL) == cBlack || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			}
			stopMotors();
			break;
		case  LEFTLBLACK + LEFTMBLACK + RIGHTMBLACK + RIGHTRBLACK:
			goBack(3);
			if (!(getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen))
			{
				motor[LMotor] = forward;
				motor[RMotor] = forward;
			}
		case MIDDLEBLACK + LEFTMBLACK + LEFTMBLACK:
		case MIDDLEBLACK + RIGHTMBLACK + RIGHTRBLACK:
		case LEFTLBLACK + LEFTMBLACK + MIDDLEBLACK + RIGHTMBLACK + RIGHTRBLACK:
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			break;
		default:
			displayTextLine(0,"State: STRAIGHT");
			writeDebugStreamLine("STRAIGHT");
			//twoEye PID??
			//motor[LMotor] = forward;
			//motor[RMotor] = forward;
			P = leftFrontM.clear - rightFrontM.clear;
			writeDebugStreamLine("P: %d", P);
			if (abs(P) < 1400)
				I = 0;
			else
				I += P;
			D = P - lastP;
			adjust = P*kP + I*kI + D*kD;
			//adjust = 0;
			motor[LMotor] = forward + adjust;
			motor[RMotor] = forward - adjust;
			break;
	}
	lastState = state;
}

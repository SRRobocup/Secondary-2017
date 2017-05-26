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
	       (int) leftFrontM.currentColor*1000 +
	       (int) middleFront.currentColor*100 +
	       (int) rightFrontM.currentColor*10 +
	       (int) rightFrontR.currentColor;
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
	(upperLimit - clamp(values[2],lowerLimit,upperLimit)) * 30 +
	(upperLimit - clamp(values[3],lowerLimit,upperLimit)) * 1 +
	(upperLimit - clamp(values[4],lowerLimit,upperLimit)) * -1 +
	(upperLimit - clamp(values[5],lowerLimit,upperLimit)) * -30 +
	(upperLimit - clamp(values[6],lowerLimit,upperLimit)) * -100 +
	(upperLimit - clamp(values[7],lowerLimit,upperLimit)) * -500;
	motor[LMotor] = tp - curr*kp;
	motor[RMotor] = tp + curr*kp;
}

void lineTrace()
{
	//0.002, 0.001, 0.0015
	const float kP = 0.0013, kI = 0.000, kD = 0.0005;
	float P,I = 0,D, lastP;
	float adjust;
	float forwardDistance = 5.6;
	bool tag = false;
	unsigned int state = getState();
	bool lastLight[5] = {leftFrontL.isLight,leftFrontM.isLight,middleFront.isLight,rightFrontM.isLight,rightFrontR.isLight};
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
			{
				playSound(soundBlip);
				state = RIGHTGREEN;
			}
			else
			{
				state = LEFTGREEN;
			}
		}
		else if (rightFrontM.currentColor == cGreen)
		{
			if (getColor(leftFrontM) == cGreen || getColor(leftFrontM) == cGreen)
				state = 02020;
			else
			if (rightFrontR.currentColor == cBlack)
			{
				playSound(soundBlip);
				state = LEFTGREEN;
			}
			else
			{
				state = RIGHTGREEN;
			}
		}
		switch (state)
		{
			case 10001:
			case 30003:
			case 3030:
			case 1030:
				state = lastTurn;
				break;
			case 30:
			case 3:
				state = RIGHTMBLACK;
				break;
			case -1:
				state = RIGHTRBLACK + RIGHTMBLACK;
				break;
		}
	}
	else
		state = 02020;

	if (state == 33 && (lastState == RIGHTGREEN || lastState == LEFTGREEN))
		state = RIGHTMBLACK + RIGHTRBLACK;
	if (state == 33000 && (lastState == RIGHTGREEN || lastState == LEFTGREEN))
		state = LEFTMBLACK + LEFTLBLACK;
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
					writeDebugStreamLine("FOUND LINE %d", getState());
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
					if (getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen)
					{
						playSound(soundException);
						stopMotors();
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
			while (!seeLine()){}
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
			resetEncoders();
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while ((getColor(leftFrontM) == cGreen || leftFrontM.currentColor == cGradient) && abs(nMotorEncoder[LMotor]) < 3*cm){writeDebugStreamLine("WAITING %d",nMotorEncoder[LMotor]);}
			if(abs(nMotorEncoder[LMotor]) >= 3*cm)
				goBack(nMotorEncoder[LMotor]/cm);
			else if (leftFrontM.currentColor == cWhite && getColor(leftFrontM) == cWhite)
			{
				playSound(soundBeepBeep);
				writeDebugStreamLine("SKIPPED GREEN");
				return;
			}
			else
				writeDebugStreamLine("LEFT COLOR: %d", leftFrontM.currentColor);
			lastTurn = RIGHTMBLACK;
			goStraight(forwardDistance);
			turnLeft(30);
			nMotorEncoder[RMotor] = 0;
			motor[LMotor] = -forward;
			motor[RMotor] = forward;
			while (getColor(middleFront) != cWhite){}
			while ((getColor(middleFront) == cWhite || getColor(middleFront) == cWhite) && nMotorEncoder[RMotor] < wheelbase*PI/6*cm){}
			if (nMotorEncoder[RMotor] >= wheelbase*PI/6*cm)
				return;
			writeDebugStreamLine("MIDDLE COLOR: %d",middleFront.currentColor);
			while (getColor(leftFrontL) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			writeDebugStreamLine("STATE: %d",getState());
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
			resetEncoders();
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			while ((getColor(rightFrontM) == cGreen || rightFrontM.currentColor == cGradient) && abs(nMotorEncoder[LMotor]) < 3*cm){writeDebugStreamLine("WAITING %d",nMotorEncoder[LMotor]);}
			if(abs(nMotorEncoder[LMotor]) >= 3*cm)
				goBack(nMotorEncoder[LMotor]/cm);
			else if (rightFrontM.currentColor == cWhite && getColor(rightFrontM) == cWhite)
			{
				playSound(soundBeepBeep);
				writeDebugStreamLine("SKIPPED GREEN");
				return;
			}
			else
				writeDebugStreamLine("RIGHT COLOR: %d", rightFrontM.currentColor);
			lastTurn = LEFTMBLACK;
			goStraight(forwardDistance);
			turnRight(30);
			nMotorEncoder[LMotor] = 0;
			motor[LMotor] = forward;
			motor[RMotor] = -forward;
			while (getColor(middleFront) != cWhite){}
			while ((getColor(middleFront) == cWhite || getColor(middleFront) == cWhite) && nMotorEncoder[LMotor] < wheelbase*PI/6*cm){}
			if (nMotorEncoder[LMotor] >= wheelbase*PI/6*cm)
				return;
			writeDebugStreamLine("MIDDLE COLOR: %d",middleFront.currentColor);
			while (getColor(rightFrontR) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack){}
			writeDebugStreamLine("STATE: %d",getState());
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
				if (getColor(rightFrontM) == cGreen || getColor(leftFrontM) == cGreen)
					return;
			}
			//lastTurn = LEFTMBLACK;
			turnLeft(90);
			if (getColor(middleFront) != cBlack) {
				motor[LMotor] = forward*2/3;
				motor[RMotor] = -forward*2/3;
				nMotorEncoder[LMotor] = 0;
				//while (getColor(middleFront) != cWhite){}
				while (!seeLine() && nMotorEncoder[LMotor] < wheelbase*PI/4*cm)
				{
					writeDebugStreamLine("ENCODER: %d" , nMotorEncoder[LMotor]);
					writeDebugStreamLine("STATE: %d",getState());
					if (nMotorEncoder[LMotor] > wheelbase*PI/8*cm)
					{
						motor[LMotor] = forward;
						motor[RMotor] = -forward;
					}
				}
			}
			else
				tag = true;
			if (nMotorEncoder[LMotor] < wheelbase*PI/4*cm || tag)
			{
				writeDebugStreamLine("FOUND A LINE");
				//while (getColor(middleFront) != cWhite){}
				//motor[LMotor] = forward;
				//motor[RMotor] = -forward;
				//while (getColor(middleFront) == cWhite){}
				//motor[LMotor] = -forward;
				//motor[RMotor] = forward;
				//if(getColor(leftFrontL) == cBlack || getColor(leftFrontM) == cBlack)
				//{
				//	playSound(soundBeepBeep);
				//	writeDebugStreamLine("SKIPPED CHECK");
				//	stopMotors();
				//	return;
				//}
				while (!tag && (getColor(rightFrontR) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack))
				{
					writeDebugStreamLine("STATE: %d",getState());
					if (leftFrontM.currentColor == cGreen|| rightFrontM.currentColor == cGreen)
						return;
					if (leftFrontM.currentColor == cBlack && getColor(leftFrontL) == cBlack)
						break;
					if (rightFrontM.currentColor == cBlack && getColor(rightFrontR) == cBlack)
						break;
				}
				if (!tag && getColor(leftFrontL) == cBlack)
					while (getColor(middleFront) == cWhite){}
				nMotorEncoder[LMotor] = 0;
				motor[LMotor] = -forward;
				motor[RMotor] = -forward;
				while ((getColor(leftFrontM) != cBlack && getColor(rightFrontM) != cBlack && leftFrontM.currentColor != cGreen && rightFrontM.currentColor != cGreen) && abs(nMotorEncoder[LMotor]) < 5*cm){}
				if (abs(nMotorEncoder[LMotor]) < 5*cm && leftFrontM.currentColor != cGreen && rightFrontM.currentColor != cGreen)
					while (getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack || leftFrontM.currentColor == cGradient || rightFrontM.currentColor == cGradient){}
				else
					writeDebugStreamLine("HAHA I FOOLED YOU");
				if (!(getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen || getColor(leftFrontL) == cGreen || getColor(rightFrontR) == cGreen))
					goStraight(abs(nMotorEncoder[LMotor])/cm);
			}
			else if (!seeLine())
			{
				while (!seeLine() && nMotorEncoder[LMotor] < wheelbase*PI/2*cm){}
				if(nMotorEncoder[LMotor] >= wheelbase*PI/2*cm)
				{
					stopMotors();
					turnLeft(170);
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
				//playSound(soundshortBlip);
				if (getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen)
					return;
			}
			//lastTurn = RIGHTMBLACK;
			turnRight(90);
			if (getColor(middleFront) != cBlack)
			{
				motor[LMotor] = -forward;
				motor[RMotor] = forward;
				nMotorEncoder[RMotor] = 0;
				//while (getColor(middleFront) != cWhite){}
				while (!seeLine() && nMotorEncoder[RMotor] < wheelbase*PI/4*cm)
				{
					writeDebugStreamLine("ENCODER: %d" , nMotorEncoder[RMotor]);
					writeDebugStreamLine("STATE: %d",getState());
					writeDebugStreamLine("");
					if (nMotorEncoder[RMotor] > wheelbase*PI/8*cm)
					{
						motor[LMotor] = -forward;
						motor[RMotor] = forward;
					}
				}
				writeDebugStreamLine("STATE: %d",getState());
			}
			else
				tag = true;
			if (nMotorEncoder[RMotor] < wheelbase*PI/4*cm || tag)
			{
				writeDebugStreamLine("FOUND A LINE");
				//while (getColor(middleFront) != cWhite){}
				//motor[LMotor] = -forward;
				//motor[RMotor] = forward;
				//while (getColor(middleFront) == cWhite){}
				//motor[LMotor] = forward;
				//motor[RMotor] = -forward;
				//if(getColor(rightFrontM) == cBlack && getColor(rightFrontR) == cBlack)
				//{
				//	playSound(soundBeepBeep);
				//	writeDebugStreamLine("SKIPPED CHECK");
				//	stopMotors();
				//	return;
				//}
				while (!tag && (getColor(leftFrontL) != cWhite || getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack))
				{
					writeDebugStreamLine("STATE: %d",getState());
					if (leftFrontM.currentColor == cGreen|| rightFrontM.currentColor == cGreen)
						return;
					if (leftFrontM.currentColor == cBlack && getColor(leftFrontL) == cBlack)
						break;
					if (rightFrontM.currentColor == cBlack && getColor(rightFrontR) == cBlack)
						break;
				}
				if (!tag && getColor(rightFrontR) == cBlack)
					while (getColor(middleFront) == cWhite){}
				nMotorEncoder[LMotor] = 0;
				motor[LMotor] = -forward;
				motor[RMotor] = -forward;
				while ((getColor(leftFrontM) != cBlack && getColor(rightFrontM) != cBlack && leftFrontM.currentColor != cGreen && rightFrontM.currentColor != cGreen) && abs(nMotorEncoder[LMotor]) < 5*cm)
				{
					writeDebugStreamLine("STATE: %d",getState());
				}
				if (abs(nMotorEncoder[LMotor]) < 5*cm && leftFrontM.currentColor != cGreen && rightFrontM.currentColor != cGreen)
					while (getColor(leftFrontM) == cBlack || getColor(rightFrontM) == cBlack || leftFrontM.currentColor == cGradient || rightFrontM.currentColor == cGradient){}
				else
					writeDebugStreamLine("HAHA I FOOLED YOU");
				writeDebugStreamLine("FINAL STATE: %d",getState());
				if (!(getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen || getColor(leftFrontL) == cGreen || getColor(rightFrontR) == cGreen))
				{
					motor[LMotor] = forward;
					motor[RMotor] = forward;
					while(abs(nMotorEncoder[LMotor]) > 10)
					{
						if (getColor(leftFrontM) == cGreen || getColor(rightFrontM) == cGreen)
							break;
					}
				}
			}
			else
			{
				writeDebugStreamLine("ME BAD");
				while (!seeLine() && nMotorEncoder[RMotor] < wheelbase*PI/2*cm){}
				if(nMotorEncoder[RMotor] >= wheelbase*PI/2*cm)
				{
					stopMotors();
					turnRight(170);
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

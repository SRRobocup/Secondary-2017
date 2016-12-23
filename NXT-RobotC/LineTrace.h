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

void lineTrace()
{
	unsigned int state = getState();
	state = leftFrontM.currentColor == cGreen ? LEFTGREEN : state;
	state = rightFrontM.currentColor == cGreen ? RIGHTGREEN : state;
	switch (state)
	{
		case NONE:
			displayTextLine(0,"State: GAP");
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
			motor[LMotor] = forward;
			motor[RMotor] = forward;
			break;
	}
}

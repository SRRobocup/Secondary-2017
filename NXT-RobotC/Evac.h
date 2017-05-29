#include "Global.h"

int numberOfBalls = 0;

void grabBall()
{
	openServo(clawServo);
	lowerClaw();
	closeServo(clawServo);
	raiseClaw();
	numberOfBalls++;
}

void dumpBalls()
{
	openServo(dumperServo);
	wait1Msec(2000);
	closeServo(dumperServo);
	numberOfBalls = 0;
}

void goStraightNoReset(float dist, int power = forward)
{
	dist *= cm;
	nMotorEncoderTarget[LMotor] =  nMotorEncoder[LMotor] + dist;
	motor[LMotor] = power;
	motor[RMotor] = power;
	while(stillRunning(LMotor)){}
	stopMotors();
}

void evacClaw()
{
	motor[LMotor] = forward;
	motor[RMotor] = forward;
	while (getColor(leftFrontL) != cSilver || getColor(rightFrontR) != cSilver){}
	//retract arm
	goStraight(3.5);
	float distLeft = getDistance(leftDist);
	float distRight = getDistance(rightDist);
	float currDist;
	int dir = distLeft < distRight ? -1 : 1; //-1 is left 1 is right
	int tp = forward;
	float firstSide, secondSide, cornerSide, sweepSide;

	currDist = dir == -1 ? distLeft : distRight;
	turnAbs(90*dir,tp);
	motor[LMotor] = tp;
	motor[RMotor] = tp;
	nMotorEncoder[LMotor] = 0;
	int checked = 1, corner = dir == -1 ? 2 : 1;
	while (checked < 3)
	{
		if (SensorValue[touch] == 1 && nMotorEncoder[LMotor] < 3*cm)
		{
			//we guessed the wrong side and got screwed
			goBack(1);
			turnAbs(180*-dir,tp);
			motor[LMotor] = tp;
			motor[RMotor] = tp;
			nMotorEncoder[LMotor] = 0;
			dir *= -1;
			currDist = dir == -1 ? distLeft : distRight;
			if (checked == 1)
				checked -= dir;
		}
		else if (SensorValue[touch] == 1 && (equals(nMotorEncoder[LMotor]/cm,87,1) || abs(nMotorEncoder[LMotor] - 117*cm) < cm))
		{
			//we hit the wall
			stopMotors();
			if(checked == 1 && abs(nMotorEncoder[LMotor] - 87*cm) < cm)
			{
				firstSide = 90;
				secondSide = 120;
			}
			else if (checked == 1)
			{
				firstSide = 120;
				secondSide = 90;
			}
			checked++;
			distLeft = getDistance(leftDist);
			distRight = getDistance(rightDist);
			dir = distLeft < distRight ? -1 : 1;
			currDist = dir == -1 ? distLeft : distRight;
			turnAbs(90*dir,tp);
			motor[LMotor] = tp;
			motor[RMotor] = tp;
			nMotorEncoder[LMotor] = 0;
		}
		else if (SensorValue[touch] == 1 && (equals(nMotorEncoder[LMotor]/cm,57,1) || equals(nMotorEncoder[LMotor]/cm,87,1)))
		{
			if (checked == 1)
			{
				if (abs(nMotorEncoder[LMotor] - 57*cm) < cm)
				{
					firstSide = 90;
					secondSide = 120;
				}
				else
				{
					firstSide = 120;
					secondSide = 90;
				}
				cornerSide = firstSide;
				sweepSide = secondSide;
			}
			else
			{
				cornerSide = secondSide;
				sweepSide = firstSide;
			}
			break;
		}
		else if (SensorValue[touch] == 1)
		{
			//heyooo we found a ball
			stopMotors();
			grabBall();
			motor[LMotor] = forward;
			motor[RMotor] = forward;
		}
		if(abs(nMotorEncoder[LMotor]-currDist*cm) < 12)
		{
			stopMotors();
			grabBall();
			motor[LMotor] = forward;
			motor[RMotor] = forward;
		}
	}

	turnAbs(180*dir,tp);
	if (numberOfBalls > 0)
		dumpBalls();
	if (checked < 3)
		goStraight(cornerSide - length);


	//now that we've found the corner, lets sweep the room
	stopMotors();
	float distOut = 0;
	motor[LMotor] = tp;
	motor[RMotor] = tp;
	nMotorEncoder[LMotor] = 0;
	bool shouldDump = false;
	LaserSensor ballSensor;
	ballSensor.address = (dir == -1 ? leftDist.address : rightDist.address);
	int offset = 0;
	while (true)
	{
		distOut = (nMotorEncoder[LMotor] + offset)/cm;
		currDist = getDistance(ballSensor);
		if (numberOfBalls >= 4 || shouldDump || (equals(distOut,sweepSide,1) && numberOfBalls > 0))
		{
			goBack(distOut,tp);
			turnAbs(90*dir,tp);
			goBack(cornerSide - length,tp);
			dumpBalls();
			goStraight(cornerSide - length,tp);
			turnAbs(90*-dir,tp);
			motor[LMotor] = tp;
			motor[RMotor] = tp;
			nMotorEncoder[LMotor] = 0;
		}
		if (currDist < 118)
		{
			goStraightNoReset(3,tp);
			offset += nMotorEncoder[LMotor];
			turnAbs(90*dir,tp);
			goStraight(currDist - 3,tp);
			grabBall();
			goBack(currDist - 3,tp);
			turnAbs(90*-dir,tp);
			nMotorEncoder[LMotor] = 0;
		}
	}
}

int fullClose = 8250;

void openSweeper()
{
	nMotorEncoder[liftMotor] = 0;
	motor[liftMotor] = -100;
	while(nMotorEncoder[liftMotor] > -fullClose){}
	motor[liftMotor] = 0;
}

void closeSweeper()
{
	nMotorEncoder[liftMotor] = 0;
	motor[liftMotor] = 100;
	while(nMotorEncoder[liftMotor] < fullClose){}
	motor[liftMotor] = 0;
}

int updateCorner(int corner)
{
	return corner - 1 == 0 ? 4 : corner - 1;
}

void turnSweep()
{
	nMotorEncoder[liftMotor] = 0;
	motor[liftMotor] = 60;
	turnLeft(40);
	while(nMotorEncoder[liftMotor] < 4000){}
	turnLeft(20);
	while(nMotorEncoder[liftMotor] < 5000){}
	turnLeft(10);
	while(nMotorEncoder[liftMotor] < 6500){}
	turnLeft(20);
	//while(nMotorEncoder[liftMotor] < 7500){}
	//turnLeft(20);
	while(nMotorEncoder[liftMotor] < fullClose){}
	turnRight(20);
	motor[liftMotor] = 0;
}

void evac()
{
	eraseDisplay();
	int forwardPower = 50;
	int startCorner = -1;
	int currentCorner = startCorner;
	int timeAdjust = 1000;
	float ultraThreshold = 8.5;
	float forwardAdjust = 1;
	float frontDistance = getDistance(frontPing);
	displayTextLine(1,"FRONT: %f", frontDistance);
	turnRight(90);
	float rightDistance = getDistance(frontPing);
	displayTextLine(2,"RIGHT: %f", rightDistance);
	turnLeft(190);
	float leftDistance = getDistance(frontPing);
	displayTextLine(3,"LEFT: %f", leftDistance);
	if(leftDistance > rightDistance)
	{
		startCorner = 2;
		motor[LMotor] = -forwardPower;
		motor[RMotor] = -forwardPower;
		wait1Msec(timeAdjust + 800);
		goStraight(forwardAdjust,forwardPower);
		stopMotors();
	}
	else
	{
		turnRight(180);
		startCorner = 1;
		motor[LMotor] = -forwardPower;
		motor[RMotor] = -forwardPower;
		wait1Msec(timeAdjust + 800);
		goStraight(forwardAdjust,forwardPower);
		stopMotors();
		turnLeft(90);
	}
	currentCorner = startCorner;
	openSweeper();
	//nSyncedMotors = synchAC;
	//nSyncedTurnRatio = 100;
	motor[RMotor] = forwardPower + 2;
	motor[LMotor] = forwardPower;
	while (getColor(middleFront) != cBlack)
	{
		displayTextLine(0,"CORNER: %d", currentCorner);
		if (getDistance(frontPing) < ultraThreshold)
		{
			delay(50);
			if (getDistance(frontPing) < ultraThreshold)
			{
				delay(50);
				if (getDistance(frontPing) < ultraThreshold)
				{
					resetSync();
					turnRight(70);
					goStraight(5);
					turnRight(30);
					goBack(3);
					motor[LMotor] = -forwardPower;
					motor[RMotor] = -forwardPower;
					wait1Msec(timeAdjust);
					currentCorner = updateCorner(currentCorner);
					//nSyncedMotors = synchAC;
					//nSyncedTurnRatio = 100;
					motor[RMotor] = forwardPower + 1;
					motor[LMotor] = forwardPower;
				}
			}
		}
		delay(100);
	}
	resetSync();
	stopMotors();
	turnSweep();
	goBack(20);
	//closeSweeper();
	//go back to entrance
}

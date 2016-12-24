#include "Global.h"
#define frontUltra 1
#define rightUltra 2
#define leftUltra 3
#define motorRight motorA
#define motorLeft motorB
void getUltraSensorValue(int port)
{
	//Get sensor values...don't know how to do.
}
void turn(int degrees)
{
	//Cant make this function till we build the robot...
}
void trace(float thresh, int pwr, int direction, int sensor) //pass in thresh, power, direction of trace (left or right), and sensor to read from.
{
	float PWeight = 0.1; //PID Variables

	float IWeight = 0.001;
	float IFade = 0.01;
	float ISum = 0.0;

	float DWeight = 0.001;
	float DPrev = 0;
	float DSlope = 0;

	float value;
	float err;

	while(1) //again...no idea when to return cause i cant read sensor values....
	{
		value = getUltraSensorValue(sensor); //get current value and error
		err = thresh - value;

		motor[motorRight] = pwr + direction*(err*PWeight + ISum*IWeight + DSlope*DWeight); //Move based on PID
		motor[motorLeft] = pwr - direction*(err*PWeight + ISum*IWeight + DSlope*DWeight);

		ISum = err + ISum*IFade; //Update PID
		DSlope = err - DPrev;
		DPrev = err;
	}
}
void obstacle(float thresh, int pwr) //pass in thresh for detection and trace, and a speed to trace
{
	if(getUltraSensorValue(frontUltra)<thresh) //if there is an obstacle
	{
		turn(-90); //turn
		if(getUltraSensorValue(leftUltra)<thresh) //if there is a wall to the left (we assume there is no wall to the right)
		{
			turn(180); //turn
			trace(thresh, pwr, 1, rightUltra); //start PID trace
		}
		else //no wall to left
		{
			trace(thresh, pwr, -1, leftUltra); //start PID trace
		}
	}
}

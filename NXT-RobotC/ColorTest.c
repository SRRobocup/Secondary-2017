//#include "common.h"
//#define DEBUG_COMMON_H
//#define DEBUG
//#define SHORT_DEBUG
#include "Global.h"

void setup()
{
	arduino = S3;
	SensorType[arduino] = sensorI2CCustom9V;
	generateColor(&leftFrontL,LEFT_FRONT,0,0,0,0,0,true);
	generateColor(&leftFrontM,LEFT_MIDDLE,0,0,0,0,0,false);
	generateColor(&middleFront,MIDDLE_FRONT,0,0,0,0,0,true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,0,0,0,0,0,false);
	generateColor(&rightFrontR,RIGHT_FRONT,0,0,0,0,0,true);
}

task main()
{
	setup();
	clearDebugStream();
	int r,g,b;
	while (true) {
		getColorRGB(leftFrontL,r,g,b);
		writeDebugStreamLine("LF COLOR %x: %d %d %d %d %f",LEFT_FRONT,r,g,b,leftFrontL.clear,b == 0 ? 0 : (float)g/b);
		displayTextLine(3,"LF: %d %d %d %d %f",r,g,b,leftFrontL.clear,b == 0 ? 0 : g/b);
		//delay(5);
		getColorRGB(leftFrontM,r,g,b);
		writeDebugStreamLine("LM COLOR %x: %d %d %d %d %f",LEFT_MIDDLE,r,g,b,leftFrontM.clear,b == 0 ? 0 : (float)g/b);
		displayTextLine(4,"LM: %d %d %d %d",r,g,b,leftFrontM.clear);
		//delay(5);
		getColorRGB(middleFront,r,g,b);
		writeDebugStreamLine("MF COLOR %x: %d %d %d %d %f",MIDDLE_FRONT,r,g,b,middleFront.clear,b == 0 ? 0 : (float)g/b);
		displayTextLine(5,"MF: %d %d %d %d",r,g,b,middleFront.clear);
		//delay(5);
		getColorRGB(rightFrontM,r,g,b);
		writeDebugStreamLine("RM COLOR %x: %d %d %d %d %f",RIGHT_MIDDLE,r,g,b,rightFrontM.clear,b == 0 ? 0 : (float)g/b);
		displayTextLine(6,"RM: %d %d %d %d",r,g,b,rightFrontM.clear);
		//delay(5);
		getColorRGB(rightFrontR,r,g,b);
		writeDebugStreamLine("RF COLOR %x: %d %d %d %d %f",RIGHT_FRONT,r,g,b,rightFrontR.clear,b == 0 ? 0 : (float)g/b);
		displayTextLine(7,"RF: %d %d %d %d",r,g,b,rightFrontR.clear);

		writeDebugStreamLine("------------------------------------------------------------------------------");
		delay(500);
	}
}

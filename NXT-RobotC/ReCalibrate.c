#include "Global.h"

void waitDisplay(char* mes)
{
	displayTextLine(4, mes);
	writeDebugStreamLine(mes);
	while(!getXbuttonValue(xButtonEnter)){delay(10);}
	wait1Msec(400);
}

float determineThresh(float x, float y)
{
	if (x > y)
		return x-(abs(x-y)*2/3);
	else
		return x+(abs(x-y)*2/3);
}

void setup()
{
	arduino = S3;
	MSLSA = S1;
	LMotor = motorC;
	RMotor = motorA;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	SensorType[arduino] = sensorI2CCustom9V;
	SensorType[MSLSA] = sensorI2CCustom9V;
	leftDist.address = LEFT_LASER;
	frontDist.address = FRONT_LASER;
	rightDist.address = RIGHT_LASER;
	frontPing.address = FRONT_PING;
	MSLSAinit(MSLSA);
	generateColor(&leftFrontL,LEFT_FRONT,0,0,0,0,0,true);
	generateColor(&leftFrontM,LEFT_MIDDLE,0,0,0,0,0,true);
	generateColor(&middleFront,MIDDLE_FRONT,0,0,0,0,0,true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,0,0,0,0,0,true);
	generateColor(&rightFrontR,RIGHT_FRONT,0,0,0,0,0,true);
}

task main()
{
	setup();
	int delete;
	waitDisplay("Place All on White");
	long white[5][2];
	getColorRGB(leftFrontL,delete,white[0][0],delete,white[0][1]);
	getColorRGB(leftFrontM,delete,white[1][0],delete,white[1][1]);
	getColorRGB(middleFront,delete,white[2][0],delete,white[2][1]);
	getColorRGB(rightFrontM,delete,white[3][0],delete,white[3][1]);
	getColorRGB(rightFrontR,delete,white[4][0],delete,white[4][1]);
	if (delete == 0)
		playSound(soundBeepBeep);
	MSLSAcalWhite(MSLSA);

	int black[5][2];
	waitDisplay("Place Array on Black");
	MSLSAcalBlack(MSLSA);

	MSLSASleep(MSLSA);
	delay(200);
	MSLSAinit(MSLSA);

	waitDisplay("Place Left and Right on Black");
	getColorRGB(leftFrontL,delete,black[0][0],delete,black[0][1]);
	getColorRGB(leftFrontM,delete,black[1][0],delete,black[1][1]);
	getColorRGB(rightFrontM,delete,black[3][0],delete,black[3][1]);
	getColorRGB(rightFrontR,delete,black[4][0],delete,black[4][1]);
	if (delete == 0)
		playSound(soundBeepBeep);

	waitDisplay("Place Middle on Black");
	getColorRGB(middleFront,delete,black[2][0],delete,black[2][1]);
	if (delete == 0)
		playSound(soundBeepBeep);

	int silver[5];
	waitDisplay("Place Left and Right on Silver");
	getColorRGB(leftFrontL,delete,delete,delete,silver[0]);
	getColorRGB(leftFrontM,delete,delete,delete,silver[1]);
	getColorRGB(rightFrontM,delete,delete,delete,silver[3]);
	getColorRGB(rightFrontR,delete,delete,delete,silver[4]);
	if (delete == 0)
		playSound(soundBeepBeep);

	waitDisplay("Place Middle on Silver");
	getColorRGB(middleFront,delete,delete,delete,silver[2]);
	if (delete == 0)
		playSound(soundBeepBeep);

	const int index = 2;
	int green[5][2] = {{900,-10},{900,-10},{900,-10},{900,-10},{900,-10}};
	float temp;
	int data[3];
	float min[] = {900, 900, 900, 900, 900};
	waitDisplay("Roll LeftL on Green");
	playSound(soundBeepBeep);
	for(int i = 0; i < 100; i++)
	{
		getColorRGB(leftFrontL,data[0],data[1],data[2],delete);
		writeDebugStreamLine("%d",data[1]);
		if (data[index] == 0)
			continue;
		temp = (float)data[1]/data[index];
		if(temp < min[0])
			min[0] = temp;
		if(data[1] < green[0][0])
			green[0][0] = data[1];
		if(data[1] > green[0][1])
			green[0][1] = data[1];
	}
	playSound(soundBlip);
	writeDebugStreamLine("");
	waitDisplay("Roll LeftM on Green");
	for(int i = 0; i < 100; i++)
	{
		getColorRGB(leftFrontM,data[0],data[1],data[2],delete);
		writeDebugStreamLine("%d",data[1]);
		if (data[index] == 0)
			continue;
		temp = (float)data[1]/data[index];
		if(temp < min[1])
			min[1] = temp;
		if(data[1] < green[1][0])
			green[1][0] = data[1];
		if(data[1] > green[1][1])
			green[1][1] = data[1];
	}
	playSound(soundBlip);
	writeDebugStreamLine("");
	waitDisplay("Roll Middle on Green");
	for(int i = 0; i < 100; i++)
	{
		getColorRGB(middleFront,data[0],data[1],data[2],delete);
		writeDebugStreamLine("%d",data[1]);
		if (data[index] == 0)
			continue;
		temp = (float)data[1]/data[index];
		if(temp < min[2])
			min[2] = temp;
		if(data[1] < green[2][0])
			green[2][0] = data[1];
		if(data[1] > green[2][1])
			green[2][1] = data[1];
	}
	playSound(soundBlip);
	writeDebugStreamLine("");
	waitDisplay("Roll RightM on Green");
	for(int i = 0; i < 100; i++)
	{
		getColorRGB(rightFrontM,data[0],data[1],data[2],delete);
		writeDebugStreamLine("%d",data[1]);
		if (data[index] == 0)
			continue;
		temp = (float)data[1]/data[index];
		if(temp < min[3])
			min[3] = temp;
		if(data[1] < green[3][0])
			green[3][0] = data[1];
		if(data[1] > green[3][1])
			green[3][1] = data[1];
	}
	playSound(soundBlip);
	writeDebugStreamLine("");
	waitDisplay("Roll RightR on Green");
	for(int i = 0; i < 100; i++)
	{
		getColorRGB(rightFrontR,data[0],data[1],data[2],delete);
		writeDebugStreamLine("%d",data[1]);
		if (data[index] == 0)
			continue;
		temp = (float)data[1]/data[index];
		if(temp < min[4])
			min[4] = temp;
		if(data[1] < green[4][0])
			green[4][0] = data[1];
		if(data[1] > green[4][1])
			green[4][1] = data[1];
	}
	playSound(soundBlip);
	writeDebugStreamLine("{%.1f,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%.1f,%.1f,%.2f,%.1f,%.1f,%.1f,%.1f,%.2f};",
	(float)(black[0][1] + white[0][1])/2,silver[0]-40,determineThresh(black[0][0],green[0][0]), determineThresh(white[0][0],green[0][1]),min[0]-0.05,
	(float)(black[1][1] + white[1][1])/2,silver[1]-40,determineThresh(black[1][0],green[1][0]), determineThresh(white[1][0],green[1][1]),min[1]-0.05,
	(float)(black[2][1] + white[2][1])/2,silver[2]-40,determineThresh(black[2][0],green[2][0]), determineThresh(white[2][0],green[2][1]),min[2]-0.05,
	(float)(black[3][1] + white[3][1])/2,silver[3]-40,determineThresh(black[3][0],green[3][0]), determineThresh(white[3][0],green[3][1]),min[3]-0.05,
	(float)(black[4][1] + white[4][1])/2,silver[4]-40,determineThresh(black[4][0],green[4][0]), determineThresh(white[4][0],green[4][1]),min[4]-0.05);
}

#include "Global.h"

void setup()
{
	arduino = S1;
	MSLSA = S3;
	LMotor = motorC;
	RMotor = motorA;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	SensorType[arduino] = sensorI2CCustom;
	SensorType[MSLSA] = sensorI2CCustom;
	leftDist.address = 0x42;
	frontDist.address = 0x43;
	rightDist.address = 0x44;
	downLeftDist.address = 0x45;
	downRightDist.address = 0x46;
	frontPing.address = 0x47;
	MSLSAinit(MSLSA);
	float values[25] = {6728.5,12072.0,711.7,2966.33,0.8,6041.0,11019.0,703.7,2429.67,0.7,12027.5,21499.0,835.0,8615.33,0.9,5020.5,9512.0,712.3,2415.00,0.8,4530.0,8231.0,694.0,1827.67, 0.8};
	generateColor(&leftFrontL,LEFT_FRONT,values[0],values[1],values[2],values[3],values[4],true);
	generateColor(&leftFrontM,LEFT_MIDDLE,values[5],values[6],values[7],values[8],values[9],true);
	generateColor(&middleFront,MIDDLE_FRONT,values[10],values[11],values[12],values[13],values[14],true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,values[15],values[16],values[17],values[18],values[19],true);
	generateColor(&rightFrontR,RIGHT_FRONT,values[20],values[21],values[22],values[23],values[24],true);
}

task main()
{
	setup();
	turnRight(90);
}

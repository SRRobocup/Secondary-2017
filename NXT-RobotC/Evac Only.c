#include "Global.h"
#include "Evac.h"

void setup()
{
	arduino = S3;
	touch = S4;
	MSLSA = S2;
	//servoController = S1;
	LMotor = motorA;
	RMotor = motorC;
	liftMotor = motorB;
	//generateServo(dumperServo,1,1700,500);
	////writeDebugStreamLine("%d", dumperServo.port);
	//generateServo(clawServo,2,1000,2000);
	//frontColorServo.address = FRONT_SERVO;
	bMotorReflected[LMotor] = true;
	bMotorReflected[RMotor] = true;
	bFloatDuringInactiveMotorPWM = false;
	SensorType[arduino] = sensorI2CCustom9V;
	SensorType[MSLSA] = sensorI2CCustom9V;
	//SensorType[servoController] = sensorI2CCustom9V;
	leftDist.address = LEFT_LASER;
	frontDist.address = FRONT_LASER;
	rightDist.address = RIGHT_LASER;
	frontPing.address = FRONT_PING;
	// 									BW			Sil		Black	White  Rat  BW		 Sil		Black	White  Rat  BW      Sil     Black White  Rat  BW     Sil    Black White  Rat   BW    Sil    Black White  Rat
	float values[25] = {4680.5,7393.0,748.3,1730.3,1.7,4522.5,7183.0,748.0,1597.0,1.55,4056.5,5958.0,754.0,44524.3,0.95,4503.0,7190.0,746.3,1854.7,1.55,3750.0,5999.0,728.0,1449.7,1.7};
	generateColor(&leftFrontL,LEFT_FRONT,values[0],values[1],values[2],values[3],values[4],false);
	generateColor(&leftFrontM,LEFT_MIDDLE,values[5],values[6],values[7],values[8],values[9],false);
	generateColor(&middleFront,MIDDLE_FRONT,values[10],values[11],values[12],values[13],values[14],true);
	generateColor(&rightFrontM,RIGHT_MIDDLE,values[15],values[16],values[17],values[18],values[19],false);
	generateColor(&rightFrontR,RIGHT_FRONT,values[20],values[21],values[22],values[23],values[24],false);
	//closeServo(dumperServo);
	//closeServo(clawServo);
	//raiseClaw();
	//motor[liftMotor] = -30;
	//wait1Msec(2000);
	//motor[liftMotor] = 0;
	//nMotorEncoder[liftMotor] = 0;
}

task main()
{
	setup();
	//goStraight(15);
	evac();
}

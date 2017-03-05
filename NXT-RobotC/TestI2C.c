#include "common.h"

task main()
{
	clearDebugStream();
	SensorType[S1] = sensorI2CCustom;
	tByteArray send;
	tByteArray recieve;
	send[0] = 3;
	send[1] = 0x08;
	send[2] = 0x00;
	send[3] = 0x02;
	for (int i=0;i<100;i++){
		writeI2C(S1,send,recieve,4);
		writeDebugStreamLine("%X %X %X %X",recieve[0],recieve[1],recieve[2],recieve[3]);
		delay(100);
	}
}

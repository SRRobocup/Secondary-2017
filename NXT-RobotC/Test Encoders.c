task main()
{
	clearDebugStream();
	bFloatDuringInactiveMotorPWM = true;
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	while (true)
	{
		writeDebugStreamLine("ENCODER A: %d", nMotorEncoder[motorA]);
		writeDebugStreamLine("ENCODER B: %d", nMotorEncoder[motorB]);
		writeDebugStreamLine("ENCODER C: %d", nMotorEncoder[motorC]);
		writeDebugStreamLine("-----------------------------------------------------------");
		delay(10);
	}
}

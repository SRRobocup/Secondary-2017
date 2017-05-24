/*
This is function library to use with mindsensors.com NXTServo module.
For use with RobotC.
V1.0
2008(c) mindsensors.com
for more info visit www.mindsensors.com

History:
When           Ahthor/Editor          Comments
05/29/08       Deepak Patil           Initial creation of the file.
03/21/11       support                Added support for Port and i2c address in API parameters
*/

/*===========================================================================
Register Summary
===================
Register Read Operation                       Write Operation
0x00    sw version                            control reg

0x01    Servo 1 speed                         Servo 1 speed
0x02    Servo 1 Position low byte in uS	      Servo 1 Position low byte in uS
0x03    Servo 1 Position hi byte in uS	      Servo 1 Position hi byte in uS

0x04    Servo 2 speed                         Servo 2 speed
0x05    Servo 2 Position low byte in uS	      Servo 2 Position low byte in uS
0x06    Servo 2 Position hi byte in uS	      Servo 2 Position hi byte in uS

0x07    Servo 3 speed                         Servo 3 speed
0x08    Servo 3 Position low byte in uS	      Servo 3 Position low byte in uS
0x09    Servo 3 Position hi byte in uS	      Servo 3 Position hi byte in uS

0x0a    Servo 4 speed                         Servo 4 speed
0x0b    Servo 4 Position low byte in uS	      Servo 4 Position low byte in uS
0x0c    Servo 4 Position hi byte in uS	      Servo 4 Position hi byte in uS

0x0d    Servo 5 speed                         Servo 5 speed
0x0e    Servo 5 Position low byte in uS	      Servo 5 Position low byte in uS
0x0f    Servo 5 Position hi byte in uS	      Servo 5 Position hi byte in uS

0x10    Servo 6 speed                         Servo 6 speed
0x11    Servo 6 Position low byte in uS	      Servo 6 Position low byte in uS
0x12    Servo 6 Position hi byte in uS	      Servo 6 Position hi byte in uS

0x13    Servo 7 speed                         Servo 7 speed
0x14    Servo 7 Position low byte in uS	      Servo 7 Position low byte in uS
0x15    Servo 7 Position hi byte in uS	      Servo 7 Position hi byte in uS

0x16    Servo 8 speed                         Servo 8 speed
0x17    Servo 8 Position low byte in uS	      Servo 8 Position low byte in uS
0x18    Servo 8 Position hi byte in uS	      Servo 8 Position hi byte in uS

0x19    battery voltage	None


================
Psudo registers
================

0x1A			servo position 1			nothing
0x1b			servo position 2			nothing
0x1c			servo position 3			nothing
0x1d			servo position 4			nothing
0x1e			servo position 5			nothing
0x1f			servo position 6			nothing
0x20			servo position 7			nothing
0x21			servo position 8			nothing

*/

#pragma SystemFile

const int kSc8CommandReg    = 0x41;
const int kSc8_speed        = 0x52;
const int kSc8_lowbyte      = 0x42;
const int kSc8_hibyte       = 0x43;

const int kSc8_Vbatt        = 0x41;


/*===================================
**
** Read the battery voltage data from
** NXTServo module (in mili-volts)
**
===================================*/
int  NXTServo_Get_Batt_V(tSensors kSc8Port, int kSc8ID)
{
  ubyte sc8Msg[5];
  const int kMsgSize       = 0;
  const int kSc8Address    = 1;
  const int kReadAddress   = 2;
	ubyte replyMsg[2];

  // Build the I2C message
  sc8Msg[kMsgSize]         = 2;
  sc8Msg[kSc8Address]      = kSc8ID ;
  sc8Msg[kReadAddress]     = kSc8_Vbatt ;

  while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  sendI2CMsg(kSc8Port, &sc8Msg[0], 1);

  while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  readI2CReply(kSc8Port, &replyMsg[0], 1);

  return(37*(0x00FF & replyMsg[0]));  // 37 is calculated from

														//supply from NXT =4700 mv /128
}

/*===================================
**
** Quick setup for servo.
** Valid values are from 50 to 250
** uses pseudo registers from 0x1A to 0x21
**
===================================*/
void NXTServo_Quick_Servo_Setup(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{

  ubyte sc8Msg[5];
  const int kMsgSize          = 0;
  const int kSc8Address       = 1;
  const int kSc8Servo         = 2;
  const int kData             = 3;

  // Build the I2C message
  sc8Msg[kMsgSize]            = 3;
  sc8Msg[kSc8Address]         = kSc8ID;
  sc8Msg[kSc8Servo]           = 0x59+servoNumber ;
  sc8Msg[kData]               = (ubyte)position ;

  while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  sendI2CMsg(kSc8Port, &sc8Msg[0], 0);

}

/*===================================
**
** Set the speed of a servo to
** given value.
** highest speed -> 0
** lowest speed -> 255
**
===================================*/

void NXTServo_SetSpeed(tSensors kSc8Port, int kSc8ID, int servoNumber, int speed)
{

  ubyte sc8Msg[5];
  const int kMsgSize        = 0;
  const int kSc8Address     = 1;
  const int kSc8Servo       = 2;
  const int kData       = 3;

  // Build the I2C message
  sc8Msg[kMsgSize]          = 4;
  sc8Msg[kSc8Address]       = kSc8ID;
  sc8Msg[kSc8Servo]         = kSc8_speed+servoNumber-1 ;
  sc8Msg[kData]             = speed ;

  while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING)
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  sendI2CMsg(kSc8Port, &sc8Msg[0], 0);

}

/*===================================
**
** Set the position of a servo to
** given value.
**
===================================*/

void NXTServo_SetPosition(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{
  NXTServo_Quick_Servo_Setup(kSc8Port, kSc8ID, servoNumber, position/10);
}

/*
 *
 *
 *
 */

/*
void NXTServo_SetPosition(tSensors kSc8Port, int kSc8ID, int servoNumber, int position)
{

  byte sc8Msg[5];
  const int kMsgSize        = 0;
  const int kSc8Address     = 1;
  const int kSc8Servo       = 2;
  const int kData_low       = 3;
  const int kData_hi        = 4;

  // Build the I2C message
  sc8Msg[kMsgSize]          = 4;
  sc8Msg[kSc8Address]       = kSc8ID;
  sc8Msg[kSc8Servo]         = kSc8_lowbyte+2*servoNumber-2 ;
  sc8Msg[kData_low]         = (ubyte)position ;
  sc8Msg[kData_hi]          = position/0x100;

  while (nI2CStatus[kSc8Port] == STAT_COMM_PENDING);
  {
   // Wait for I2C bus to be ready
  }
  // when the I2C bus is ready, send the message you built
  sendI2CMsg(kSc8Port, sc8Msg[0], 0);

}

*/

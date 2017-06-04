#include "Global.h"

int power = 127;

uint8_t getArrayState() {
  int val[ARRAY_SIZE];
  uint8_t ret = 0;
  getArrayValues(val);
  for (int i = 0; i < sizeof(val)/sizeof(int); i++)
    ret = (val < arrayThreshold) | (ret << 1); 
  return ret;
}

void arrayPID() {
  int P, I, D;
  float kP = 0;
  float kI = 0;
  float kD = 0;
  float adjust = 0;
  P = getWeightedArrValue();
  adjust = P * kP;
  LMotor.setPower(power - adjust);
  RMotor.setPower(power + adjust);
}

void lineTrace() {
  leftColor.getColor();
  rightColor.getColor();
  uint8_t arrayState = getArrayState();
  float forwardDistance = 6;
  if (leftColor.currentColor == cGreen && rightColor.currentColor == cGreen) {
    goStraight(forwardDistance, power);
    turnRight(180, power);
  } else if (leftColor.currentColor == cGreen) {
    LMotor.setPower(power);
    LMotor.setPower(power);
    while (leftColor.getColor() != cBlack && leftColor.currentColor != cWhite){}
    if (leftColor.currentColor == cWhite)
      return;
    LMotor.setPower(-power);
    RMotor.setPower(power);
    while (!seeLine()){}
    stopMotors();
    turnToMiddleArray();
  } else if (rightColor.currentColor == cGreen) {
    LMotor.setPower(power);
    LMotor.setPower(power);
    while (rightColor.getColor() != cBlack && rightColor.currentColor != cWhite){}
    if (rightColor.currentColor == cWhite)
      return;
    LMotor.setPower(power);
    RMotor.setPower(-power);
    while (!seeLine()){}
    stopMotors();
    turnToMiddleArray();
  }
  switch (arrayState) {
    case B00011111:
//    case B001111:
      goStraight(3, power);
      if (seeLine())
        return;
      goStraight(forwardDistance - 3, power);
      LMotor.setPower(power);
      RMotor.setPower(-power);
      while (!seeLine()){}
      stopMotors();
      turnToMiddleArray();
      break;
    case B11111000:
//    case B111100:
      goStraight(3, power);
      if (seeLine())
        return;
      goStraight(forwardDistance - 3, power);
      LMotor.setPower(-power);
      RMotor.setPower(power);
      while (!seeLine()){}
      stopMotors();
      turnToMiddleArray();
      break;
//    case B11111111:
//    case B111111:
//      
//      break;
    default:
      arrayPID();
      break;
  }
}


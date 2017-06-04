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
  int val[8];
  getArrayValues(val);
  const float kP;
  const float kI;
  const float kD;
  float P = 0, I = 0, D = 0;
  #ifdef MSLSA
  kP = 1;
  kI = 0;
  kD = 0;
  #else
  kP = 0.1;
  kI = 0;
  kD = 0;
  #endif
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
    goStraight(forwardDistance, power);
    turnLeft(90,power);
  } else if (rightColor.currentColor == cGreen) {
    goStraight(forwardDistance, power);
    turnRight(90,power);
  }
  switch (arrayState) {
    case B00011111:
    case B001111:
      goStraight(3, power);
      if (seeLine())
        return;
      goStraight(forwardDistance - 3, power);
      turnRight(90, power);
      break;
    case B11111000:
    case B111100:
      goStraight(3, power);
      if (seeLine())
        return;
      goStraight(forwardDistance - 3, power);
      turnLeft(90, power);
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


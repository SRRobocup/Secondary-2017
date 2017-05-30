#include "Global.h"

uint8_t getArrayState() {
  int val[8];
  uint8_t ret = 0;
  getArrayValues(val,8);
  for (int i = 0; i < sizeof(val)/sizeof(int); i++)
    ret = (val < 50) | (ret << 1); 
  return ret;
}

void arrayPID() {
  int val[8];
  getArrayValues(val,8);
  //PID
}

void lineTrace() {
  leftColor.getColor();
  rightColor.getColor();
  uint8_t arrayState = getArrayState();
  if (leftColor.currentColor == cGreen && rightColor.currentColor == cGreen) {
    //dead end
  } else if (leftColor.currentColor == cGreen) {
    //left green negotiation
  } else if (rightColor.currentColor == cGreen) {
    //right green negotiation
  }
  switch (arrayState) {
    
  }
}


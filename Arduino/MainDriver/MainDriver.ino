#include "Global.h"

void leftInterruptFunction() {
  LMotor.encoderValue++;
}

void rightInterruptFunction() {
  RMotor.encoderValue++;
}

void setup() {
  leftLaser.begin();
  rightLaser.begin();
  leftColor.begin();
  rightColor.begin();
  LMotor.resetEncoder();
  RMotor.resetEncoder();
  attachInterrupt(digitalPinToInterrupt(LMotor.getPort()),leftInterruptFunction,RISING);
  attachInterrupt(digitalPinToInterrupt(RMotor.getPort()),rightInterruptFunction,RISING);
}

void loop() {
  lineTrace();
  if (frontPing.getDistance() < 7)
    obstacle();
  if (false) //silver
    evac();
}

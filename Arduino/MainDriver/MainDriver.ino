#include "Global.h"
bool ledState = false;
long lastTime;

void leftInterruptFunction() {
  LMotor.encoderValue++;
}

void rightInterruptFunction() {
  RMotor.encoderValue++;
}

void setup() {
  Serial.begin(115200);
  TWBR = 12;
  Wire.begin();
  initQik();
  leftLaser.begin();
  rightLaser.begin();
  leftColor.begin();
  rightColor.begin();
  LMotor.resetEncoder();
  RMotor.resetEncoder();
  attachInterrupt(digitalPinToInterrupt(LMotor.getPort()),leftInterruptFunction,RISING);
  attachInterrupt(digitalPinToInterrupt(RMotor.getPort()),rightInterruptFunction,RISING);
  lastTime = millis();
  pinMode(LED, OUTPUT);
}

void loop() {
  if (millis() - lastTime >= 1000) {
    ledState = !ledState;
    digitalWrite(LED, ledState ? HIGH : LOW);
  }
  lineTrace();
  if (frontPing.getDistance() < 7)
    obstacle();
  if (leftColor.getColor() == cSilver && rightColor.getColor() == cSilver) { //silver
    //double check
    evac();
  }
}

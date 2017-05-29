#include "Global.h"
//#include "Obstacle.cpp"
//#include "Evac.cpp"
//#include "LineTrace.cpp"

Motor LMotor(0);
Motor RMotor(1);
LaserSensor leftLaser(5);
PingSensor frontPing(4);
LaserSensor rightLaser(7);
ColorSensor leftColor = ColorSensor(1, 0, 0, 0, 0, false);
ColorSensor rightColor = ColorSensor(2, 0, 0, 0, 0, false);

void setup() {
  
}

void loop() {
  lineTrace();
  if (frontPing.getDistance() < 7)
    obstacle();
  if (false)
    evac();
}

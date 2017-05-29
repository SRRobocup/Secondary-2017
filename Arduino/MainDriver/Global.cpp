#include "Global.h"

Adafruit_TCS34725 colorSensorI2C = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
TCA9548A mux = TCA9548A();
VL53L0X longRangeI2C = VL53L0X();

ColorSensor::ColorSensor(int p, int bThresh, int wThresh, float rat, int bwThresh, bool l)
  :port(port)
  ,blackThreshold(bThresh)
  ,whiteThreshold(wThresh)
  ,greenRatio(rat)
  ,bwThreshold(bwThresh)
  ,isLight(l)
{
  r = 0;
  g = 0;
  b = 0;
  c = 0;
  currentColor = cInvalid;
}

boolean ColorSensor::begin() {
  mux.select(port);
  colorSensorI2C.begin();
}

void ColorSensor::getColorRGB(int &r, int &g, int &b, int &c) {
  mux.select(port);
  colorSensorI2C.getRawDataEx(&r,&g,&b,&c);
  this->r = r;
  this->g = g;
  this->b = b;
  this->c = c;
}

Color ColorSensor::getColor() {
  if (port < 0 || port > 7)
      return cInvalid;
  Color curr;
  mux.select(port);
  int red,green,blue,clear;
  do {
    getColorRGB(red,green,blue,clear);
    if (isLight) {
      curr = (Color) (clear < bwThreshold);
      break;
    }
    if (green < blackThreshold) {
      curr = cBlack;
      break;
    }
    if (green > whiteThreshold) {
      curr = cWhite;
      break;
    }
    if ((float)green/blue > greenRatio) {
      curr = cGreen;
      break;
    }
    //writeDebugStreamLine("GRADIENT %d: %d %d", sensor.address, green, red);
    curr = cGradient;
  } while (false);
  currentColor = curr;
  return curr;
}

LaserSensor::LaserSensor(int p)
  :port(p)
{
}

void LaserSensor::begin() {
  mux.select(port);
  longRangeI2C.init();
  longRangeI2C.setTimeout(500);
  longRangeI2C.setMeasurementTimingBudget(20000);
  longRangeI2C.startContinuous();
}

float LaserSensor::getDistance() {
  mux.select(port);
  return longRangeI2C.readRangeContinuousMillimeters()/(float) 10;
}

PingSensor::PingSensor(int p)
  :pin(p)
{
}

float PingSensor::getDistance()
{
  long duration;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);
  return (float)duration / 29 / 2;
}

Motor::Motor(int p)
  :port(p)
  ,encoderValue(0)
{
}

void Motor::setPower(int power) {
  if (port == 0)
    ;
  else
    ;
}

void Motor::resetEncoder() {
  encoderValue = 0;
}

void getArrayValues(int val[]) {
  
}


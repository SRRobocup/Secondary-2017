#include "Global.h"

Adafruit_TCS34725 colorSensorI2C = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
TCA9548A mux = TCA9548A();
VL53L0X longRangeI2C = VL53L0X();

Motor LMotor = Motor(0);
Motor RMotor = Motor(1);
LaserSensor leftLaser = LaserSensor(5);
PingSensor frontPing = PingSensor(4);
LaserSensor rightLaser = LaserSensor(7);
ColorSensor leftColor = ColorSensor(1, 0, 0, 0, 0, false);
ColorSensor rightColor = ColorSensor(2, 0, 0, 0, 0, false);

/**
 * Constructor for ColorSebsor
 */
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

/**
 * Starts the color sensor's polling
 */
boolean ColorSensor::begin() {
  mux.select(port);
  colorSensorI2C.begin();
}

/**
 * Acts as wrapper for the getColorEx of the TCS34725 Sensor
 */
void ColorSensor::getColorRGB(int &r, int &g, int &b, int &c) {
  mux.select(port);
  colorSensorI2C.getRawDataEx(&r,&g,&b,&c);
  this->r = r;
  this->g = g;
  this->b = b;
  this->c = c;
}

/**
 * Runs the rgb value through the threshold to determine color
 */
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

/**
 * Initializes laser sensor with port p
 */
LaserSensor::LaserSensor(int p)
  :port(p)
{
}

/**
 * Starts continuous polling of laser sensor
 */
void LaserSensor::begin() {
  mux.select(port);
  longRangeI2C.init();
  longRangeI2C.setTimeout(500);
  longRangeI2C.setMeasurementTimingBudget(20000);
  longRangeI2C.startContinuous();
}

/**
 * Reads the laser for value
 */
float LaserSensor::getDistance() {
  mux.select(port);
  return longRangeI2C.readRangeContinuousMillimeters()/(float) 10;
}

/**
 * Initializes the ping sensor with signal pin p
 */
PingSensor::PingSensor(int p)
  :pin(p)
{
}

/**
 * Gets distance of Ping sensor using digital pin
 */
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

/**
 * Intializes the motor with port p
 */
Motor::Motor(int p)
  :port(p)
  ,encoderValue(0)
{
}

/**
 * Sets power of motor
 */
void Motor::setPower(int power) {
  if (port == 0)
    Motor::setM0Power(power);
  else
    Motor::setM1Power(power);
}

/**
 * Sets encoder count back to 0
 */
void Motor::resetEncoder() {
  encoderValue = 0;
}

/**
 * Gets the current running to the motors
 */
float Motor::getCurrent() {
  if (port == 0)
    return Motor::getM0Current();
  else
    return Motor::getM1Current();
}

int Motor::getPort() {
  return port;
}

/**
 * Gets the values of the light array
 */
int getArrayValues(int val[]) {
  Wire.beginTransmission(0x14 >> 1);
  Wire.write(0x42);
  int error = Wire.endTransmission();
  if (error != 0)
    return error;
  Wire.requestFrom(0x14 >> 1,8);
  for (int i = 0; Wire.available(); i++)
    val[i] = Wire.read();
  return error;
}

void Motor::setM0Power(int power) {
  
}

void Motor::setM1Power(int power) {
  
}

float Motor::getM0Current() {
  return 0;
}

float Motor::getM1Current() {
  return 0;
}

void initQik() {
  
}

/**
 * Ensures that the given value is within the lower and upper bound
 */
float clamp(float value, float lowerBound, float upperBound) {
  if (value < lowerBound)
    return lowerBound;
  if (value > upperBound)
    return upperBound;
  return value;
}


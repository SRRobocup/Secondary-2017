#include <QTRSensors.h>
#include "Global.h"
#include <SoftwareSerial.h>
#include <Adafruit_TCS34725.h>
#include <PololuQik.h>
#include <TCA9548A.h>
#include <VL53L0X.h>
#include <Wire.h>
#include "Pinmap.h"


#define NUM_SENSORS             8  // number of sensors used
#define TIMEOUT                 2500  // average 4 analog samples per sensor reading
#define EMITTER_PIN             12  // emitter is controlled by digital pin 2


Adafruit_TCS34725 colorSensorI2C = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
TCA9548A mux = TCA9548A();
VL53L0X longRangeI2C = VL53L0X();
PololuQik2s12v10 motorController = PololuQik2s12v10(10, 9, 11);

QTRSensorsRC qtra((unsigned char[]) {4, 5, 6, 7, 8, A0, A1, A2}, 
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);


void  initMotorController(){
  motorController.init();
}

Motor LMotor = Motor(0);
Motor RMotor = Motor(1);
LaserSensor leftLaser = LaserSensor(5);
PingSensor frontPing = PingSensor(4);
LaserSensor rightLaser = LaserSensor(7);
ColorSensor leftColor = ColorSensor(1, 0, 0, 0, 0, 0, 0);
ColorSensor rightColor = ColorSensor(2, 0, 0, 0, 0, 0, 0);

float wheelbase = 10;
const float encPerCM = 900 / (PI * 4);
#ifdef MSLSA
const int arrayThreshold = 0;
#else
const int arrayThreshold = 700;
#endif


/**
   Constructor for ColorSebsor
*/
ColorSensor::ColorSensor(int p, int bThresh, int wThresh, float rat, int lMin, int lMax, int temp)
  : port(port)
  , blackThreshold(bThresh)
  , whiteThreshold(wThresh)
  , greenRatio(rat)
  , luxMin(lMin)
  , luxMax(lMax)
  , tempThreshold(temp)
{
  r = 0;
  g = 0;
  b = 0;
  c = 0;
  currentColor = cInvalid;
}

/**
   Starts the color sensor's polling
*/
bool ColorSensor::begin() {
  mux.select(port);
  colorSensorI2C.begin();
}

/**
   Acts as wrapper for the getColorEx of the TCS34725 Sensor
*/
void ColorSensor::getColorRGB(int &r, int &g, int &b, int &c) {
  mux.select(port);
  colorSensorI2C.getRawDataEx(&r, &g, &b, &c);
  this->r = r;
  this->g = g;
  this->b = b;
  this->c = c;
}

uint16_t ColorSensor::getLux() {
  mux.select(port);
  int del;
  getColorRGB(del,del,del,del);
  return colorSensorI2C.calculateLux(r, g, b);
}

uint16_t ColorSensor::getColorTemperature() {
  mux.select(port);
  int del;
  getColorRGB(del,del,del,del);
  return colorSensorI2C.calculateColorTemperature(r, g, b);
}

/**
   Runs the rgb value through the threshold to determine color
*/
Color ColorSensor::getColor() {
  if (port < 0 || port > 7)
    return cInvalid;
  Color curr;
  mux.select(port);
  int red, green, blue, clear;
  do {
    getColorRGB(red, green, blue, clear);
    if (green < blackThreshold) {
      curr = cBlack;
      break;
    }
    if (green > whiteThreshold) {
      curr = cWhite;
      break;
    }
    if ((float)green / blue > greenRatio) {
      curr = cGreen;
      break;
    }
    //writeDebugStreamLine("GRADIENT %d: %d %d", sensor.address, green, red);
    curr = cGradient;
  } while (false);
  currentColor = curr;
  return curr;
}

Color ColorSensor::getSilver() {
  uint16_t temp = getColorTemperature();
  uint16_t lux = getLux();
  if (lux < luxMin)
    return cBlack;
  if (lux > luxMax || temp > tempThreshold)
    return cWhite;
  return cSilver;
}

/**
   Initializes laser sensor with port p
*/
LaserSensor::LaserSensor(int p)
  : port(p)
{
}

/**
   Starts continuous polling of laser sensor
*/
void LaserSensor::begin() {
  mux.select(port);
  longRangeI2C.init();
  longRangeI2C.setTimeout(500);
  longRangeI2C.setMeasurementTimingBudget(20000);
  longRangeI2C.startContinuous();
}

/**
   Reads the laser for value
*/
float LaserSensor::getDistance() {
  mux.select(port);
  return longRangeI2C.readRangeContinuousMillimeters() / (float) 10;
}

/**
   Initializes the ping sensor with signal pin p
*/
PingSensor::PingSensor(int p)
  : pin(p)
{
}

/**
   Gets distance of Ping sensor using digital pin
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
   Intializes the motor with port p
*/
Motor::Motor(int p)
  : port(p)
  , encoderValue(0)
{
}

/**
   Sets power of motor
*/
void Motor::setPower(int power) {
  if (port == 0)
    Motor::setM0Power(power);
  else
    Motor::setM1Power(power);
}

/**
   Sets encoder count back to 0
*/
void Motor::resetEncoder() {
  encoderValue = 0;
}

/**
   Gets the current running to the motors
*/
int Motor::getCurrent() {
  if (port == 0)
    return Motor::getM0Current();
  else
    return Motor::getM1Current();
}

int Motor::getPort() {
  return port;
}

/**
   Gets the values of the light array
*/
int getArrayValues(int val[]) {
  int startValue = 0;
  int error = 0;
  /*if (sizeof(val) != (sizeof(int) * ARRAY_SIZE))
  {
    Serial.println("invalid");
    return 0;
  }*/
#ifdef MSLSA
  Wire.beginTransmission(0x14 >> 1);
  Wire.write(0x42);
  error = Wire.endTransmission();
  if (error != 0)
    return error;
  Wire.requestFrom(0x14 >> 1, 8);
  memset(0, val, sizeof(int)*ARRAY_SIZE);
  for (int i = startValue; Wire.available(); i++)
    val[i] = Wire.read();
  while (Wire.available())
    Wire.read();
#else
qtra.read(val);
for(int i = 0; i < 8; i++)
{
 // Serial.print(val[i]);
 // Serial.print('\t');
}
//Serial.println();
#endif
  return error;
}
 
float getWeightedArrValue() {
  int val[ARRAY_SIZE];
  getArrayValues(val);
  float weightedVal= val[0] * 5
         + val[1] * .4
         + val[2] * .03
         + val[3] * .001
         
         + val[4] * .001
         + val[5] * .03
         + val[6] * .4
         + val[7] * 5;
   
   return weightedVal;
}

void Motor::setM0Power(int power) {
  //Serial.println(__LINE__);
  //Serial.println(power);
  if (power == 0)
    motorController.setM0Brake(127);
  else {
    motorController.setM0Brake(0);
    motorController.setM0Speed(power);
  }
}

void Motor::setM1Power(int power) {
  //Serial.println(__LINE__);
  //Serial.println(power);
  if (power == 0)
    motorController.setM1Brake(127);
  else {
    motorController.setM1Brake(0);
    motorController.setM1Speed(power);
  }
}

int Motor::getM0Current() {
  return motorController.getM0Current();
}

int Motor::getM1Current() {
  return motorController.getM1Current();
}

void initQik() {
  motorController.init(38400);
}

void stopMotors() {
  LMotor.setPower(0);
  RMotor.setPower(0);
}
/**
   Ensures that the given value is within the lower and upper bound
*/
float clamp(float value, float lowerBound, float upperBound) {
  if (value < lowerBound)
    return lowerBound;
  if (value > upperBound)
    return upperBound;
  return value;
}

void goStraight(float distance, int power) {
  float ticks = distance * encPerCM;
  LMotor.resetEncoder();
  LMotor.setPower(power);
  RMotor.setPower(power);
  while (LMotor.encoderValue < ticks) {}
  stopMotors();
}

void goBack(float distance, int power) {
  float ticks = distance * encPerCM;
  LMotor.resetEncoder();
  LMotor.setPower(-power);
  RMotor.setPower(-power);
  while (LMotor.encoderValue < ticks) {}
  stopMotors();
}

void turnRight(float degrees, int power) {
  float ticks = wheelbase * PI * (degrees / 360) * encPerCM;
  LMotor.resetEncoder();
  LMotor.setPower(power);
  RMotor.setPower(-power);
  while (LMotor.encoderValue < ticks) {}
  stopMotors();
}

void turnLeft(float degrees, int power) {
  float ticks = wheelbase * PI * (degrees / 360) * encPerCM;
  LMotor.resetEncoder();
  LMotor.setPower(-power);
  RMotor.setPower(power);
  while (LMotor.encoderValue < ticks) {}
  stopMotors();
}

bool seeLine() {
  int val[ARRAY_SIZE];
  getArrayValues(val);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (val[i] < arrayThreshold)
      return true;
  return false;
}

void turnToMiddleArray() {
  float P, I, D;
  float kP = 0;
  float kI = 0;
  float kD = 0;
  float adjust = 0;
  do {
    P = getWeightedArrValue();
    adjust = P * kP;
    LMotor.setPower(-adjust);
    RMotor.setPower(adjust);
  } while (abs(P) > 20);
  stopMotors();
}

int buttonPressed() {
  int thresh[5] = {600, 650, 700,  800, 900 };
  int k;
  int input = analogRead(ADKEYBOARD);
  for (k = 0; k < 5; k++)
    if (input < thresh[k])
      return k;
  if (k >= 5)k = -1;  // No valid key pressed
  return k;
}


#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <Adafruit_TCS34725.h>
#include <TCA9548A.h>
#include <VL53L0X.h>
#include <Wire.h>

enum Color{
  cWhite = 0,
  cBlack = 1,
  cGreen = 2,
  cGradient = 3,
  cSilver = 4,
  cInvalid = 5
};

class ColorSensor {
  public:
    bool isLight;
    int r,g,b,c;
    int port;
    Color currentColor;
    ColorSensor(int port, int blackThreshold, int whiteThreshold, float greenRatio, int bwThreshold, bool isLight);
    Color getColor();
    void getColorRGB(int &r, int &g, int &b, int &c);
    boolean begin();
  private:
    int blackThreshold;
    int whiteThreshold;
    float greenRatio;
    int bwThreshold;
};

class LaserSensor {
  public:
    void begin();
    LaserSensor(int port);
    float getDistance();
  private:
    int port;
};

class PingSensor {
  public:
    PingSensor(int pin);
    float getDistance();
  private:
    int pin;
};

class Motor {
  public:
    Motor(int port);
    volatile int encoderValue;
    void resetEncoder();
    void setPower(int power);
  private:
    int port;
};

extern Motor LMotor;
extern Motor RMotor;
extern LaserSensor leftLaser;
extern PingSensor frontPing;
extern LaserSensor rightLaser;
extern ColorSensor leftColor;
extern ColorSensor rightColor;

void getArrayValues(int val[]);

void obstacle();
void lineTrace();
void evac();

#endif

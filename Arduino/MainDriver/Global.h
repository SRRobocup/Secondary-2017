#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <Arduino.h>
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
    int r,g,b,c;
    int port;
    Color currentColor;
    ColorSensor(int port, int blackThreshold, int whiteThreshold, float greenRatio, int luxThreshold, int tempThreshold);
    Color getColor();
    void getColorRGB(int &r, int &g, int &b, int &c);
    uint16_t getLux();
    uint16_t getColorTemperature();
    bool begin();
  private:
    int blackThreshold;
    int whiteThreshold;
    float greenRatio;
    int luxThreshold;
    int tempThreshold;
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
    int getCurrent();
    int getPort();
  private:
    static void setM0Power(int power);
    static void setM1Power(int power);
    static int getM0Current();
    static int getM1Current();
    int port;
};

extern Motor LMotor;
extern Motor RMotor;
extern LaserSensor leftLaser;
extern PingSensor frontPing;
extern LaserSensor rightLaser;
extern ColorSensor leftColor;
extern ColorSensor rightColor;

int getArrayValues(int val[], int n);
float clamp(float value, float lowerBound, float upperBound);
void initQik();
void goStraight(float distance, int power);
void goBackward(float distance, int power);
void turnLeft(float degrees, int power);
void turnRight(float degrees, int power);

void obstacle();
void lineTrace();
void evac();

#endif

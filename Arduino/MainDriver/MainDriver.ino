#include <Servo.h>

#include <TCA9548A.h>
#include <Adafruit_TCS34725.h>
#include "C:\Users\ethan_000\Documents\Git Repos\RCJ Secondary 2017\Register.h"

#include <VL53L0X.h>

#define HIGH_SPEED
//#define DEBUG

VL53L0X longRange = VL53L0X();
VL53L0X longRange5 = VL53L0X(10);

TCA9548A mux = TCA9548A();

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Servo frontCRV;

const int pingPins[] = {7,3,9};
const int buttons[2] = {6,7};
volatile uint16_t currentTag = 0;
volatile uint8_t currentCommand = 0;
volatile uint8_t bytesToSend = 0;
volatile byte buff[8] = {0,0,0,0,0,0,0,0};
volatile byte lastSent[8] = {0,0,0,0,0,0,0,0};
volatile int servoPosition = 0;
int pingBuffer = 0;
int lastPing = 0;
uint16_t red, blue, green, clear;
bool set = false;
const int frontPingPin = 10;
const int frontServoPin = 11;
boolean isTriggered = false;
volatile boolean isInterrupted = false;

void setup() {
  Serial.begin(115200);

  pinMode(buttons[0],INPUT_PULLUP);
  pinMode(buttons[1],INPUT_PULLUP);
  
  Wire.begin(0x08 >> 1); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  frontCRV.attach(frontServoPin);
  frontCRV.write(90);

//  TWBR = 12;
  // init all sensors
  for (int i = 0; i < 5; i++) {
    mux.select(i);
    colorSensor.begin();
  }
  for (int i = 5; i < 8; i++) {
    mux.select(i);
    longRange.init();
    longRange.setTimeout(500);
    longRange.setMeasurementTimingBudget(20000);
    longRange.startContinuous();
  }
  Serial.print("START CODE: ");
  Serial.println(mux.disable());
//  longRange5.init();
//  longRange5.setTimeout(500);
//  longRange5.setMeasurementTimingBudget(20000);
//  longRange5.setAddress(0x53);
//  longRange5.startContinuous();
}

//get first byte and set to data and discard all other bytes
void receiveEvent(int bytesReceived) {
  isInterrupted = true;
  currentTag = Wire.read();
//  if (currentTag == FRONT_SERVO) {
//    servoPosition = Wire.read() << 8;
//    servoPosition = Wire.read() | servoPosition;
//  }
  for (int i = 1; Wire.available(); i++) {
    currentCommand = Wire.read();
  }
}

//send data buffer
void requestEvent() {
  byte realBuff[8];
  for (int i = 0; i < 8; i++) {
    realBuff[i] = buff[i];
    buff[i] = 0xFF;
  }
  Wire.write(realBuff,bytesToSend);
}

unsigned long pulseInLongEx(uint8_t pin, uint8_t state, unsigned long timeout)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  uint8_t stateMask = (state ? bit : 0);
  
  unsigned long startMicros = micros();
  
  while ((*portInputRegister(port) & bit) == stateMask) {
    if (micros() - startMicros > timeout || isInterrupted)
      return 0;
  }

  while ((*portInputRegister(port) & bit) != stateMask) {
    if (micros() - startMicros > timeout || isInterrupted)
      return 0;
  }

  unsigned long start = micros();
  
  while ((*portInputRegister(port) & bit) == stateMask) {
    if (micros() - startMicros > timeout || isInterrupted)
      return 0;
  }
  return micros() - start;
}

void flushBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void split(volatile byte* buff, int x) {
  buff[0] = lowByte(x);
  buff[1] = highByte(x);
}

void printArr(volatile byte* buff, int n) {
  for(int i = 0; i < n; i++)
    Serial.print(buff[i],HEX);
  Serial.println();
}

void loop() {
  //based on read from receive event, gather data from sensors
  int tempPosition;
  switch (currentTag) {
    case COMMAND_REG:
      switch (currentCommand)
      {
        case 0x02:
          buff[0] = 0x12;
          buff[1] = 0x34;
          buff[2] = 0x56;
          buff[3] = 0x78;
          bytesToSend = 4;
          break;
        case 0x03: //stall for send
          break;
      }
      break;
    case LEFT_LASER:
    case FRONT_LASER:
    case RIGHT_LASER:
//      if (currentTag - LEFT_LASER == 0) {
//        split(&buff[0], longRange5.readRangeContinuousMillimeters());
//      } else 
      {
        mux.select(5 + (currentTag - LEFT_LASER));
        split(&buff[0], longRange.readRangeContinuousMillimeters());
  //      status = (data == 8190);
        mux.disable();
      }
      bytesToSend = 2;
      set = true;
      break;
//    case 0x45:
//    case 0x46:
//      mux.select(4 - (currentCommand - 0x45));
//      data = shortRange.readRangeSingleMillimeters();
//      status = (data == 8190);
//      bytesToSend = 2;
//      break;
//    case LEFT_PING:
    case FRONT_PING:
//    case RIGHT_PING:
      split(&buff[0], pingBuffer);
      bytesToSend = 2;
      set = true;
      break;
    case LEFT_FRONT:
    case LEFT_MIDDLE:
    case MIDDLE_FRONT:
    case RIGHT_MIDDLE:
    case RIGHT_FRONT:
      mux.select(currentTag - LEFT_FRONT);
      colorSensor.getRawDataEx(&red,&blue,&green,&clear);
      #ifdef DEBUG
        Serial.print(currentTag, HEX); Serial.print(": ");
        Serial.print(red); Serial.print(" ");
        Serial.print(green); Serial.print(" ");
        Serial.print(blue); Serial.print(" ");
        Serial.print(clear); Serial.println();
      #endif
      split(&buff[0], clear);
      split(&buff[2], blue);
      split(&buff[4], green);
      split(&buff[6], red);
      bytesToSend = 8;
      mux.disable();
      set = true;
      break;
    case FRONT_SERVO:
      tempPosition = servoPosition;
      frontCRV.write(tempPosition);
      set = true;
      break;
    case TOUCH_SENSOR: 
      buff[0] = isTriggered;
      bytesToSend = 1;
      set = true;
  }
  if (set) {
    Serial.print("SENT TAG: ");
    Serial.print(currentTag, HEX);
    Serial.print("DATA: ");
    printArr(buff,bytesToSend);
    set = false;
    currentTag = -1;
  }
  if (lastSent[0] != 0)
    printArr(lastSent,8);
  //reset tag
  //Serial.println(bytesToSend);
//  isTriggered = (digitalRead(buttons[0]) == HIGH) || (digitalRead(buttons[1]) == HIGH);
  
  pinMode(pingPins[1], OUTPUT);
  digitalWrite(pingPins[1], LOW);
  delayMicroseconds(2);
  digitalWrite(pingPins[1], HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPins[1], LOW);
  
  pinMode(pingPins[1], INPUT);
  lastPing = pingBuffer;
  pingBuffer = pulseInLongEx(pingPins[1], HIGH, 1000000);
  #ifdef DEBUG
    Serial.print(lastPing); Serial.print(" "); Serial.print(pingBuffer); Serial.print(" "); Serial.println(isInterrupted);
  #endif
  if (isInterrupted) {
    pingBuffer = lastPing;
    isInterrupted = false;
    return;
  }
  delay(2);
  
//  if (Serial.available() > 1)
//  {
//    Serial.print("Recieved "); Serial.print(Serial.available()); Serial.println(" bytes");
//    Serial.flush();
//    char buff[3];
//    int i = Serial.readBytes(buff,2);
//    buff[i] = '\0';
//    Serial.print(buff);
//    currentTag = strtol(buff, NULL, 16);
//    Serial.print(" ");
//    if (Serial.available() > 2 && Serial.read() == ' ') {
//      char buff[3];
//      int i = Serial.readBytes(buff,2);
//      buff[i] = '\0';
//      Serial.print(buff);
//      currentCommand = strtol(buff, NULL, 16);
//    }
//    Serial.println();
//    flushBuffer();
//  }
}

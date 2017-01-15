#include <TCA9548A.h>
#include <Adafruit_TCS34725.h>
#include <VL6180X.h>
#include <VL53L0X.h>

#define HIGH_SPEED

VL53L0X front = VL53L0X(3);
VL53L0X left = VL53L0X(4);
VL53L0X right = VL53L0X(5);

VL6180X downLeft = VL6180X(6);
VL6180X downRight = VL6180X(7);

TCA9548A colors = TCA9548A();

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725();

const int pingPins[] = {7,8,9};
volatile uint16_t currentTag = 0;
volatile uint8_t currentCommand = 0;
volatile uint64_t data = 0;
volatile uint8_t bytesToSend = 0;
int pingBuffer[] = {0,0,0};
uint16_t red, blue, green, clear;
bool status = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08 >> 1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  TWBR = 12;
  // init all sensors
  colors.select(0);
  downLeft.init();
  downRight.init();
  front.init();
  left.init();
  right.init();
  downLeft.configureDefault();
  downLeft.setTimeout(500);
  downRight.configureDefault();
  downRight.setTimeout(500);
#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  front.setMeasurementTimingBudget(20000);
  left.setMeasurementTimingBudget(20000);
  right.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  front.setMeasurementTimingBudget(200000);
  left.setMeasurementTimingBudget(200000);
  right.setMeasurementTimingBudget(200000);
#endif
}

//get first byte and set to data and discard all other bytes
void receiveEvent(int bytesReceived) {
  int temp;
  currentTag = Wire.read();
  for (int i = 1; i < bytesReceived; i++) {
    currentCommand = Wire.read();
  }
}

//send data buffer
void requestEvent() {
  byte buff[8];
  buff[7] = (data >> 56) & 0xFF;
  buff[6] = (data >> 48) & 0xFF;
  buff[5] = (data >> 40) & 0xFF;
  buff[4] = (data >> 32) & 0xFF;
  buff[3] = (data >> 24) & 0xFF;
  buff[2] = (data >> 16) & 0xFF;
  buff[1] = (data >> 8) & 0xFF;
  buff[0] = (data) & 0xFF;
  Wire.write(buff,bytesToSend);
}

void loop() {
  //based on read from receive event, gather data from sensors
  switch (currentTag) {
    case 0x00:
      switch (currentCommand)
      {
        case 0x00:
          front.setMeasurementTimingBudget(20000);
          break;
        case 0x01:
          left.setMeasurementTimingBudget(20000);
          break;
        case 0x02:
          right.setMeasurementTimingBudget(20000);
          break;
        case 0x03:
          front.setMeasurementTimingBudget(200000);
          break;
        case 0x04:
          left.setMeasurementTimingBudget(200000);
          break;
        case 0x05:
          right.setMeasurementTimingBudget(200000);
          break;
      }
      break;
    case 0x42:
      data = left.readRangeSingleMillimeters();
      status = left.timeoutOccurred();
      bytesToSend = 2;
      break;
    case 0x43:
      data = front.readRangeSingleMillimeters();
      status = front.timeoutOccurred();
      bytesToSend = 2;
      break;
    case 0x44:
      data = right.readRangeSingleMillimeters();
      status = right.timeoutOccurred();
      bytesToSend = 2;
      break;
    case 0x45:
      data = downLeft.readRangeSingleMillimeters();c
      status = downLeft.timeoutOccurred();
      bytesToSend = 2;
      break;
    case 0x46:
      data = downRight.readRangeSingleMillimeters();
      status = downRight.timeoutOccurred();
      bytesToSend = 2;
      break;
    case 0x47:
    case 0x48:
    case 0x49:
      data = pingBuffer[currentTag - 0x47];
      bytesToSend = 2;
      break;
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
      colors.select(currentTag - 0x50);
      colorSensor.getRawDataEx(&red,&blue,&green,&clear);
      data = red << 48 | blue << 32 | green << 16 | clear;
      bytesToSend = 8;
      break;
  }
  //if no convergence, set to ridiculous number
  if (status) {
    data = 32767;
    status = false;
  }
  //reset tag
  currentTag = -1;
  for (int i = 0; i < 3; i++) {
    pinMode(pingPin[i], OUTPUT);
    digitalWrite(pingPin[i], LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin[i], HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin[i], LOW);

    pinMode(pingPin[i], INPUT);
    pingBuffer[i] = pulseIn(pingPin[i], HIGH);
  }
}

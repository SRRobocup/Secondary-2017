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

volatile uint16_t currentTag = 0;
volatile uint64_t data = 0;
volatile uint8_t bytesToSend = 0;
uint16_t red, blue, green, clear;
bool status = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08 >> 1);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  // init all sensors
  downLeft.init();
  downRight.init();
  front.init();
  left.init();
  right.init();
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
    temp = Wire.read();
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
      data = downLeft.readRangeSingleMillimeters();
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
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
      colors.select(currentTag - 0x47);
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
  currentTag = 0;
}

#include <Adafruit_TCS34725.h>
#include <VL6180X.h>
#include <VL53L0X.h>

#define HIGH_SPEED

VL53L0X front = VL53L0X(3);
VL53L0X left = VL53L0X(4);
VL53L0X right = VL53L0X(5);

VL6180X downLeft = VL6180X(6);
VL6180X downRight = VL6180X(7);

volatile uint16_t currentTag = 0;
volatile uint16_t data = 0;
bool status = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08 >> 1);
  Wire.onRequest(receiveEvent);
  Wire.onReceive(requestEvent);
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
  downLeft.setMeasurementTimingBudget(20000);
  downRight.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  front.setMeasurementTimingBudget(200000);
  left.setMeasurementTimingBudget(200000);
  right.setMeasurementTimingBudget(200000);
  downLeft.setMeasurementTimingBudget(200000);
  downRight.setMeasurementTimingBudget(200000);
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
  byte[2] buff;
  buff[0] = (data >> 8) & 0xFF;
  buff[1] = (data) & 0xFF;
  Wire.write(buff,2);
}

void loop() {
  //based on read from receive event, gather data from sensors
  switch (currentTag) {
    case 0x42:
      data = left.readRangeSingleMillimeters();
      status = left.timeoutOccurred();
      break;
    case 0x43:
      data = front.readRangeSingleMillimeters();
      status = front.timeoutOccurred();
      break;
    case 0x44:
      data = right.readRangeSingleMillimeters();
      status = right.timeoutOccurred();
      break;
    case 0x45:
      data = downLeft.readRangeSingleMillimeters();
      status = downLeft.timeoutOccurred();
      break;
    case 0x46:
      data = downRight.readRangeSingleMillimeters();
      status = downRight.timeoutOccurred();
      break;
  }
  //if no convergence, set to ridiculous number
  if (status) {
    data = 32767;
  }
  //reset tag
  currentTag = 0;
}

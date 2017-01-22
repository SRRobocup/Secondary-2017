#include <TCA9548A.h>
#include <Adafruit_TCS34725.h>
#include <VL6180X.h>
#include <VL53L0X.h>

#define HIGH_SPEED

VL53L0X longRange = VL53L0X();

VL6180X shortRange = VL6180X();

TCA9548A mux = TCA9548A();

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
  for (int i = 0; i < 5; i++) {
    mux.select(i);
    longRange.init();
    shortRange.init();
    colorSensor.init();
  }
#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  longRange.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  longRange.setMeasurementTimingBudget(200000);
#endif
  mux.disable();
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
  if(currentTag <= 0x57) {
    byte buff[8];
    buff[7] = (data >> 56) & 0xFF;
    buff[6] = (data >> 48) & 0xFF;
    buff[5] = (data >> 40) & 0xFF;
    buff[4] = (data >> 32) & 0xFF;
    buff[3] = (data >> 24) & 0xFF;
    buff[2] = (data >> 16) & 0xFF;
    buff[1] = (data >> 8) & 0xFF;
    buff[0] = (data) & 0xFF;
  } else {
    buff[1] = (pingBuffer[currentTag - 0x58] >> 8) & 0xFF;
    buff[0] = (pingBuffer[currentTag - 0x58]) & 0xFF;
    bytesToSend = 2;
  }
  Wire.write(buff,bytesToSend);
}

void loop() {
  //based on read from receive event, gather data from sensors
  switch (currentTag) {
    case 0x00:
      switch (currentCommand)
      {
        case 0x01:
          mux.disable();
          break;
      }
      break;
    case 0x42:
    case 0x43:
    case 0x44:
      mux.select(2 - 0x44);
      data = longRange.readRangeSingleMillimeters();
      status = (data == 8190);
      bytesToSend = 2;
      break;
    case 0x45:
    case 0x46:
      mux.select(4 - 0x46);
      data = shortRange.readRangeSingleMillimeters();
      status = (data == 8190);
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
      mux.select(currentTag - 0x50);
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

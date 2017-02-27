#include <TCA9548A.h>
#include <Adafruit_TCS34725.h>

#include <VL53L0X.h>

#define HIGH_SPEED

VL53L0X longRange = VL53L0X();

TCA9548A mux = TCA9548A();

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int pingPins[] = {7,8,9};
volatile uint16_t currentTag = 0;
volatile uint8_t currentCommand = 0;
volatile uint8_t bytesToSend = 0;
volatile byte buff[8] = {0,0,0,0,0,0,0,0};
int pingBuffer[] = {1,2,3};
uint16_t red, blue, green, clear;
bool status = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08 >> 1); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

return;

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
  
//#if defined HIGH_SPEED
//  // reduce timing budget to 20 ms (default is about 33 ms)
//  
//#elif defined HIGH_ACCURACY
//  // increase timing budget to 200 ms
//  longRange.setMeasurementTimingBudget(200000);
//#endif
//  longRange.startContinuous();
  Serial.print("ERROR CODE: ");
  Serial.println(mux.disable());
  //mux.select(7);
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
  byte buf[] = {0x42};
  Wire.write(buf,1);
return;
  byte realBuff[8];
  for (int i = 0; i < 8; i++)
    realBuff[i] = buff[i];
  Wire.write(realBuff,bytesToSend);
}

void flushBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void split(volatile byte* buff, int x) {
  buff[0] = (x >> 8) & 0xFF;
  buff[1] = x & 0xFF;
}

void printArr(volatile byte* buff, int n) {
  for(int i = 0; i < n; i++)
    Serial.print(buff[i],HEX);
  Serial.println();
}

void loop() {

  //based on read from receive event, gather data from sensors
  switch (currentTag) {
    case 0x00:
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
    case 0x42:
    case 0x43:
    case 0x44:
      mux.select(5 + (currentCommand - 0x42));
      split(&buff[0], longRange.readRangeSingleMillimeters());
//      status = (data == 8190);
      bytesToSend = 2;
      mux.disable();
      break;
//    case 0x45:
//    case 0x46:
//      mux.select(4 - (currentCommand - 0x45));
//      data = shortRange.readRangeSingleMillimeters();
//      status = (data == 8190);
//      bytesToSend = 2;
//      break;
    case 0x47:
    case 0x48:
    case 0x49:
      split(&buff[0], pingBuffer[currentTag - 0x47]);
      bytesToSend = 2;
      mux.disable();
      break;
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
      mux.select(currentTag - 0x50);
      colorSensor.getRawDataEx(&red,&blue,&green,&clear);
      split(&buff[0], clear);
      split(&buff[2], green);
      split(&buff[4], blue);
      split(&buff[6], red);
      bytesToSend = 8;
      mux.disable();
      break;
  }
  //if no convergence, set to ridiculous number
//  Serial.print("SENT: ");
//  printLL(sent);
//  if (status) {
//    byte
//    status = false;
//  }
  if (currentTag != -1) {
    Serial.print(currentTag, HEX);
    Serial.print(" , ");
    printArr(buff,bytesToSend);
  }
  //reset tag
  currentTag = -1;
//  Serial.println(bytesToSend);
  if (Serial.available() > 1)
  {
    Serial.print("Recieved "); Serial.print(Serial.available()); Serial.println(" bytes");
    Serial.flush();
    char buff[3];
    int i = Serial.readBytes(buff,2);
    buff[i] = '\0';
    Serial.print(buff);
    currentTag = strtol(buff, NULL, 16);
    Serial.print(" ");
    if (Serial.available() > 2 && Serial.read() == ' ') {
      char buff[3];
      int i = Serial.readBytes(buff,2);
      buff[i] = '\0';
      Serial.print(buff);
      currentCommand = strtol(buff, NULL, 16);
    }
    Serial.println();
    flushBuffer();
  }
//  for (int i = 0; i < 3; i++) {
//    pinMode(pingPin[i], OUTPUT);
//    digitalWrite(pingPin[i], LOW);
//    delayMicroseconds(2);
//    digitalWrite(pingPin[i], HIGH);
//    delayMicroseconds(5);
//    digitalWrite(pingPin[i], LOW);
//
//    pinMode(pingPin[i], INPUT);
//    pingBuffer[i] = pulseIn(pingPin[i], HIGH);
//  }
}

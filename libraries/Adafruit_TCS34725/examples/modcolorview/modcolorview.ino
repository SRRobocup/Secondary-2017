#include <Wire.h>
#include "Adafruit_TCS34725.h"



#define RANGE(x,min,max)  (((x) > (min)) && ((x) < (max))) 

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Wire.begin();
  TWBR = 12;  // @400khz - comment this out to reduce speed to 100khz
  Serial.begin(9600);
  Serial.println("Color View Test!");
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read
}

void loop() {
  long count=0;
  long end = millis()+10000;

  while (end > millis()) {
    uint16_t clear = 0, red = 0, green = 0, blue = 0;

    if (int ret = tcs.getRawDataEx(&red, &green, &blue, &clear)){
      Serial.print("ERROR - ");
      Serial.println(ret);
     }

    if (!RANGE(clear,2940,2990)  || !RANGE(green,1240,1290) ){
      Serial.print("C:\t"); Serial.print(clear);
      Serial.print("\tR:\t"); Serial.print(red);
      Serial.print("\tG:\t"); Serial.print(green);
      Serial.print("\tB:\t"); Serial.println(blue);
    }
    count++;
  }
  Serial.println("---------------");
  Serial.println(count);
//  delay(3000);
}



#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//C:	2327	R:	756	G:	958	B:	466	536933
//C:	2313	R:	753	G:	953	B:	465	536933
//C:	2528	R:	811	G:	1055	B:	511


void setup() {
  Wire.begin();
  TWBR = 12;
  Serial.begin(9600);
  Serial.println("Color View Test!");
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
  
  tcs.setInterrupt(false);      // turn on LED
    delay(60);  // takes 50ms to read
  
}


#define RANGE(x,min,max)  (((x) > (min)) && ((x) < (max))) 
void loop() {
  long count=0;
  long end = millis()+10000;

  while (end > millis()) {
    uint16_t clear = 0, red = 0, green = 0, blue = 0;

    tcs.getRawDataEx(&red, &green, &blue, &clear);

 //   tcs.setInterrupt(true);  // turn off LED

//C:	2826	R:	897	G:	1189	B:	574

    if (!RANGE(clear,2840,2879)  || !RANGE(green,1200,1230) ){
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



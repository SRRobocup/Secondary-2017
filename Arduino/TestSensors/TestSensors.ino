#include <TCA9548A.h>
#include <Adafruit_TCS34725.h>
#include <VL6180X.h>
#include <VL53L0X.h>

#define HIGH_SPEED

VL53L0X longRange = VL53L0X();

TCA9548A mux = TCA9548A();

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(115200);
  Wire.begin(); 

//digitalWrite(A4,LOW);
//digitalWrite(A5,LOW);
  //TWBR = 12;
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
  }
  Serial.println("Done init");
//  mux.disable();
}

void loop() {
  uint16_t r,g,b,c,dist;
  for (int i = 0; i < 8; i++) {
    mux.select(i);
    if (i < 5) {
      colorSensor.getRawDataEx(&r,&g,&b,&c);
      Serial.print("Color "); Serial.print(i);
      Serial.print(": R: "); Serial.print(r);
      Serial.print(" G: "); Serial.print(g);
      Serial.print(" B: "); Serial.print(b);
      Serial.print(" C: "); Serial.println(c);
    } else {
      Serial.print("Laser "); Serial.print(i);
      Serial.print(": "); Serial.println(longRange.readRangeSingleMillimeters());
    }
  }
  mux.disable();
}

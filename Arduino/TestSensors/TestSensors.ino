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

  //  digitalWrite(A4,LOW);
  //  digitalWrite(A5,LOW);
  //  TWBR = 12;
  // init all sensors
//  delay(5000);
//  Wire.beginTransmission(0x70);
//  Serial.println(Wire.endTransmission());
//  delay(500);
//  Wire.beginTransmission(0x70);
//  Serial.println(Wire.endTransmission());
  Serial.println("BEGIN");
  for (int i = 0; i < 5; i++) {
    Serial.print("ERROR: "); Serial.println(mux.select(i));
    Serial.flush();
    colorSensor.begin();
    Serial.print("Color Sensor "); Serial.print(i); Serial.println(" Begun");
    Serial.flush();
  }
  Wire.beginTransmission(0x70);
  Serial.println(Wire.endTransmission());

  
  for (int i = 5; i < 8; i++) {
    if (i == 6)
      continue;
    Serial.println(mux.select(i));
    Serial.println("I am here");

    longRange.init();
    Serial.println("I am here");

    longRange.setTimeout(500);
Serial.println("I am here");
    longRange.setMeasurementTimingBudget(20000);
Serial.println("I am here");
    longRange.startContinuous();
Serial.println("I am here");
  }
  Serial.println("Done init");
  Serial.print("ERROR: "); Serial.println(mux.disable());
  Serial.flush();
}

void loop() {
  uint16_t r = 0, g = 0, b = 0, c = 0, dist;
  for (int i = 0; i < 8; i++) {
    if (i == 6)
      continue;
    mux.select(i);
    if (i < 5) {
      colorSensor.getRawDataEx(&r, &g, &b, &c);
      Serial.print("Color "); Serial.print(i);
      Serial.print(": R: "); Serial.print(r);
      Serial.print(" G: "); Serial.print(g);
      Serial.print(" B: "); Serial.print(b);
      Serial.print(" C: "); Serial.println(c);
    } else
    {
      Serial.print("Laser "); Serial.print(i);
      Serial.print(": "); Serial.println(longRange.readRangeContinuousMillimeters());
    }
    mux.disable();
  }
//  delay(500);
}

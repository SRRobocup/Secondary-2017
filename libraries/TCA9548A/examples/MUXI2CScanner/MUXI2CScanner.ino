#include <TCA9548A.h>
#include <Wire.h>

TCA9548A mux = TCA9548A();

void setup()
{
    Wire.begin();
    Serial.begin(115200);
    Serial.println("TCA9548A I2C Address Scanner");
}

void loop()
{
  Serial.println("Scanner Begin");
  byte error;
  int nDevices = 0;
  for (byte port = 0; port < 8; port++, nDevices = 0)
  {
    mux.select(port);
    Serial.println("MUX Port: " + port);
    for(byte address = 1; address < 127; address++ ) 
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0)
      {
        Serial.print("  I2C device found at address 0x");
        address<<=1;
        if (address<16)
          Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("  !");
  
        nDevices++;
      }
      else if (error==4) 
      {
        Serial.print("  Unknow error at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.println(address,HEX);
      }
    }
    if (nDevices == 0)
      Serial.println("  No I2C devices found\n");
    else
      Serial.println("  Port " + (String)port + "is done\n--------------------");
  }
}


#include "PololuQik.h"
#include <SoftwareSerial.h>

PololuQik2s12v10 qik(10, 9, 11);
volatile int ticks = 0;
void countTicks()
{
  
   ticks++;  
 
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  pinMode(10, INPUT);
  qik.init();
  attachInterrupt(digitalPinToInterrupt(2), countTicks, RISING);
 qik.setM0Speed(125);
 qik.setM1Speed(125);
}

void loop() {
  // put your main code here, to run repeatedly:

 //countTicks();
  Serial.println(ticks);
  
  
  
 if(ticks < 400)
 {}
 else
 {
    qik.setM0Brake(127);
    qik.setM1Brake(127);
}
  
  
}

#include <Wire.h>
void setup()

{
   pinMode(13, OUTPUT); 
    Wire.begin();
}

void loop()
{


Wire.beginTransmission(0x38);     //Begin the transmission to PCF8574AP
Wire.write(0);                                //Send the data to PCF8574
Wire.endTransmission(); 
delay(500);
Wire.beginTransmission(0x38);     //Begin the transmission to PCF8574
Wire.write(255);                                //Send the data to PCF8574
Wire.endTransmission(); 
delay(500);
digitalWrite(13,HIGH);
delay(500);
digitalWrite(13,LOW);
delay(500);
}

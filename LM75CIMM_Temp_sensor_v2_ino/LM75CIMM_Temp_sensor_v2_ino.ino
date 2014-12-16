#include "Wire.h"
#define cn75address 0x48 // with pins 5~7 set to GND, the device address is 0x48

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
const int kty81pin = A2;
int R = 2680;


int i=0;
void setup()
{
 pinMode(kty81pin, INPUT); 
 pinMode(8,OUTPUT); 
 Wire.begin(); // wake up I2C bus
 Serial.begin(9600);
  lcd.begin(16, 2);
  //Serial.begin(9600);
//  lcd.print("LM75CiMM-5");
   lcd.setCursor(0,1);
    lcd.print("Temp = ");
}
void getLM75Cdata(byte *a, byte *b)
{
 // move the register pointer back to the first register
 Wire.beginTransmission(cn75address); // "Hey, CN75 @ 0x48! Message for you"
 Wire.write(0); // "move your register pointer back to 00h"
 Wire.endTransmission(); // "Thanks, goodbye..."
// now get the data from the CN75
 Wire.requestFrom(cn75address, 2); // "Hey, CN75 @ 0x48 - please send me the contents of your first two registers"
 *a = Wire.read(); // first received byte stored here
 *b = Wire.read(); // second received byte stored here
}
void showLM75Cdata()
{
 digitalWrite(8,HIGH);
 byte aa,bb;
 float temperature=0;
 getLM75Cdata(&aa,&bb);
 if (aa>127) // check for below zero degrees
 {
 temperature=((aa-128)*-1);
 if (bb==128) // check for 0.5 fraction
 {
 temperature-=0.5;
 }
 } 
 else // it must be above zero degrees
 {
 temperature=aa;
 if (bb==128) // check for 0.5 fraction
 {
 temperature+=0.5;
 }
 }
lcd.setCursor(0,0);
lcd.print("                ");
lcd.setCursor(0,0);
lcd.print("LM75CIMM-5");
 lcd.setCursor(7,1);
 lcd.print(temperature,1);
 lcd.print(" C");
 delay(100);
 digitalWrite(8,LOW);
 }

/******
float get_kty81_temp()
{
int k;
float tempmedie = 0;
float sumatemp;
for(int k=0;k<=100;k++){

int temp = analogRead (kty81pin);
float ukty = 5*temp/1023.0;
float a = 0.00001874 * 1000;
float b = 0.007884 * 1000;
float c = 1000 - R * ukty / (5 - ukty) ;
float delta = b * b - 4 * a * c; 
float delta1 = sqrt (delta) ;
float x2 = (-b + delta1) / (2 * a);
float TEMP1 = x2 + 25;
sumatemp = sumatemp + TEMP1; 
} 
   tempmedie = sumatemp / 100; 
   
 lcd.setCursor(7,1);
 lcd.print(tempmedie,1);
 lcd.print(" C");
 delay(100);
}
*/


float get_kty81_temp_v2()
{
digitalWrite(8,HIGH);
float tempmedie = 0;
float sumatemp;
float ukty;
int temp;
   // facem o bucla sa citim 100 de valori ale temperaturii
   for (int k=1; k <= 10; k++){
 // citire valoare pe intrarea analogica 
  int temp = analogRead(kty81pin); 
  // tensiunea pe senzorul KTY81-110 este
  float ukty = 5 * temp / 1023.0 ;


// definim coeficientii ecuatiei de gradul 2

// coeficintul lui dT^2 este B x R25
float a = 0.00001874*2000;
// coeficientul lui dT este A x R25
float b = 0.007884*2000;
// coeficientul liber kty81-210 has 2000oms resistance
float c = 2000 - R * ukty / (5 - ukty);
// calculam pe delta
float delta = b * b - 4 * a * c;
// extragem radical din delta
float delta1 = sqrt (delta);
// calculam solutia
float x2 =(-b + delta1)/(2 * a);
// temperatura fata de 0 grade Celsius
float temp1 = x2 + 25 ;
// aduna temperatura cu suma valoarilor anterioare
 sumatemp = sumatemp + temp1;
// se mai odihneste si el 100ms 
 delay(10);
   }
   tempmedie = sumatemp / 10;
 // aplicam corectie fina si la rezultat
  // tempmedie = tempmedie + 0.3;

lcd.setCursor(0,0);
lcd.print("                ");
lcd.setCursor(0,0);
lcd.print("KTY81-210");
lcd.setCursor(12,0);
lcd.print(temp);
lcd.setCursor(7,1);
 lcd.print(tempmedie,1);
 lcd.print(" C");
 delay(10);
 digitalWrite(8,LOW);

}

void loop()
{

showLM75Cdata(); delay(1000);

get_kty81_temp_v2(); delay(1000);


//digitalWrite(8,LOW);
 i++;
 
}





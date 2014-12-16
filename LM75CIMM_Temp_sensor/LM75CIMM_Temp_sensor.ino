
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#include <Wire.h>

#define LM75_ADDRESS 0x48;

int i=0;
int k=1;
int a;

void setup()
{
   Wire.begin(); 
  lcd.begin(16, 2);
  //Serial.begin(9600);
  lcd.print("LM75CiMM-5");
  delay(500);
  lcd.setCursor(0,1);
  
}

void loop()
{
while (k>0)
{
  while (i<17)
  {
  lcd.setCursor(i,1);
  lcd.print("-");
  delay(50);
  i++;
  }
  i=16;
  while (i>=0)
  {
  lcd.setCursor(i,1);
  lcd.print(" ");
  delay(50);
  i--;
  }
k--;
}
//****************************** start **************************//

Wire.requestFrom(LM75_ADDRESS, 2);    // request 1 bytes from slave device #1

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);





}


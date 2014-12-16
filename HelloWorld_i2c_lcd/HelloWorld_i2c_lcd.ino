#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//PCF8574AP ima adresu 0x38 (0111000 - A0 A1 A2 na GND)

LiquidCrystal_I2C lcd(0x38,16,2);  // set the LCD address to 0x38 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  //lcd.backlight();
  //lcd.print("Hello, world!");
}

void loop()
{
  
  int i=0;
    while(i<2)
    {
    lcd.setCursor(0,i);
    lcd.print("Hello!");
    delay(500);
    //if((i==2)||(i==3)){lcd.setCursor(-4,i);} ovo je za 16x4 display
    
    i++;
    }
    
    
       
    lcd.clear();
    delay(500);
    
}


#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
//LiquidCrystal lcd(3, 2, 7, 6, 5, 4);
// RS EN D4 D5 D6 D7
// rw to GBD
int i=0;
int k=1;
int a;

void setup()
{

  lcd.begin(16, 2);
  //Serial.begin(9600);
  lcd.print("LCD Test");
  delay(500);
  lcd.setCursor(0,1);
}

void loop()
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

//lcd.noDisplay();
delay(2000);
//lcd.display();

}



#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int i=0;
int k=1;
int a;

void setup()
{

  lcd.begin(16, 4);
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

}


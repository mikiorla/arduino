
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int i=0;
int k=1;
int a;

void setup()
{

  pinMode(A2, INPUT);
  lcd.begin(16, 2);
  //Serial.begin(9600);
  lcd.print("MMA7631 Test");
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

lcd.setCursor(0,1);
a = analogRead(A2);
a = map(a,0,1023,0,5000);
lcd.print(a);
delay(100);

}


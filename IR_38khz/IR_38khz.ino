
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);


#include <GraphSeries.h>
#define SendPlot 


int i=0;
int k=1;
int a;
int ir_c = 0;

void setup()
{

  pinMode(9,OUTPUT);
  //pinMode(2,INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("IR Test");
  //delay(500);
  lcd.setCursor(0,1);
  attachInterrupt(0, ir_count, FALLING);
}

void sendPlotData(String seriesName, float data)
{
  Serial.print("{");
  Serial.print(seriesName);
  Serial.print(",T,");
  Serial.print(data);
  Serial.println("}");
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

digitalWrite(9,LOW);

lcd.setCursor(0,1);
tone(8,38000);
//delay(2000);
//tone(8,0);
//delay(2000);

a = digitalRead(9);
lcd.print(a);
//delay(100);
lcd.setCursor(9,0);
lcd.print(ir_c);

#ifdef SendPlot
  sendPlotData("A2 Input",a);
#endif
}

void ir_count()
{
ir_c++;  
digitalWrite(9,HIGH);
delay(1000);
}

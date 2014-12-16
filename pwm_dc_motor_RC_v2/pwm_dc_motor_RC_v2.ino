




// TCCR1B = TCCR1B & 0b11111000 | 0x01 ; http://playground.arduino.cc/Code/PwmFrequency
/*
/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
*/

#include <GraphSeries.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int MOSFET_Q3= 9;// the pin that the Motor1 FW is attached to
const int MOSFET_Q4_PWM = 10;
const int MOSFET_Q2 = 12;
const int MOSFET_Q1_PWM = 11;
const int LED = 13;
const int LED_RW = 8;
const int RC_FW = A3;
const int RC_LEFT = A4;
const int RC_RIGHT = A2;
const int RC_RW = A5;

int clear_lcd = 300;
int GO_FW = 0;
int GO_RW = 0;
int GO_LEFT = 0;
int GO_RIGHT = 0;
int PWM_DUTY = 25;


int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Press START");
 //lcd.setCursor(8, 0); //col,row
  //lcd.print("----");
  //Serial.begin(9600);

  pinMode(RC_FW, INPUT);
  pinMode(RC_RW, INPUT);
  pinMode(RC_LEFT, INPUT);
  pinMode(RC_RIGHT, INPUT);
  
  pinMode(MOSFET_Q3, OUTPUT);
  pinMode(MOSFET_Q4_PWM, OUTPUT);
  pinMode(MOSFET_Q2, OUTPUT);
  pinMode(MOSFET_Q1_PWM, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(LED_RW,OUTPUT);
  TCCR1B = TCCR1B & 0b11111000 | 0x04 ;

  digitalWrite(LED,HIGH);
  digitalWrite(LED_RW,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  digitalWrite(LED_RW,LOW);

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


GO_FW = analogRead(RC_FW); 
GO_RW = analogRead(RC_RW); 
GO_LEFT = analogRead(RC_LEFT); 
GO_RIGHT = analogRead(RC_RIGHT); 

sendPlotData("Forward",GO_FW);
sendPlotData("Reverse",GO_RW );
sendPlotData("Left",GO_LEFT);
sendPlotData("Right",GO_RIGHT);


if ((GO_LEFT > 900) & (GO_RIGHT < 100)) //na levo povecaj PWM
{
delay(20);
if ((GO_LEFT > 900) & (GO_RIGHT < 100))
{ 

if ( PWM_DUTY == 250 ) {PWM_DUTY = 0;}
else {
  PWM_DUTY += 25;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press START"); 
  lcd.setCursor(0, 1);
  lcd.print("PWM "); 
  lcd.print(PWM_DUTY);
    }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press START"); 
  lcd.setCursor(0, 1);
  lcd.print("PWM "); 
  lcd.print(PWM_DUTY);
}
  }


while ((GO_FW > 800) & (GO_RW < 100))//napred
{
digitalWrite(LED,HIGH);
digitalWrite(LED_RW,LOW);

digitalWrite(MOSFET_Q3,HIGH);
analogWrite(MOSFET_Q4_PWM,PWM_DUTY);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Forward");
lcd.setCursor(0, 1);
lcd.print("PWM "); 
lcd.print(PWM_DUTY);


GO_RW = analogRead(RC_RW); 
GO_LEFT = analogRead(RC_LEFT); 
GO_FW = analogRead(RC_FW);
GO_RIGHT = analogRead(RC_RIGHT); 

sendPlotData("Forward",GO_FW);
sendPlotData("Reverse",GO_RW );
sendPlotData("Left",GO_LEFT);
sendPlotData("Right",GO_RIGHT);


}


while((GO_RW > 800) & (GO_FW < 100)) //nazad
{
digitalWrite(LED,LOW);
digitalWrite(LED_RW,HIGH);

digitalWrite(MOSFET_Q2,HIGH);
analogWrite(MOSFET_Q1_PWM,PWM_DUTY);

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Forward");
lcd.setCursor(0, 1);
lcd.print("PWM "); 
lcd.print(PWM_DUTY);

GO_RW = analogRead(RC_RW); 
GO_LEFT = analogRead(RC_LEFT); 
GO_FW = analogRead(RC_FW);
GO_RIGHT = analogRead(RC_RIGHT); 

sendPlotData("Forward",GO_FW);
sendPlotData("Reverse",GO_RW );
sendPlotData("Left",GO_LEFT);
sendPlotData("Right",GO_RIGHT);
}



analogWrite(MOSFET_Q1_PWM,0);
analogWrite(MOSFET_Q4_PWM,0);
digitalWrite(MOSFET_Q2,LOW);
digitalWrite(MOSFET_Q3,LOW);

digitalWrite(LED,LOW);
digitalWrite(LED_RW,LOW);
 

}  
 




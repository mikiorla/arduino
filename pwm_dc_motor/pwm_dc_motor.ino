


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




#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int MOSFET_Q1= 9;// the pin that the Motor1 FW is attached to
const int MOSFET_Q4 = 10;
const int START_PIN = 13;
const int STOP_PIN = 12;
const int LED = 11;
const int OPTO = 8;

int PWM_DUTY = 0;


int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin


void setup()
{
  lcd.begin(16, 2);
  lcd.print("Press START");
 //lcd.setCursor(8, 0); //col,row
  //lcd.print("----");
  //Serial.begin(9600);
  pinMode(START_PIN, INPUT);
  pinMode(STOP_PIN, INPUT);
  pinMode(MOSFET_Q1, OUTPUT);
  pinMode(MOSFET_Q4, OUTPUT);
  pinMode(LED,OUTPUT);
  TCCR1B = TCCR1B & 0b11111000 | 0x04 ;
}

void loop()
{

int start_motor = digitalRead(START_PIN); 
int stop_motor = digitalRead(STOP_PIN); 


if (start_motor) {
digitalWrite(LED,HIGH);

if ( PWM_DUTY < 250 ) {PWM_DUTY+=50;}

lcd.clear();
lcd.setCursor(0, 1);
lcd.print("PWM "); 
lcd.print(PWM_DUTY);

delay(300); //debounce
}

if (stop_motor) {
if (PWM_DUTY != 0){
PWM_DUTY -= 50;
lcd.clear();
lcd.setCursor(0, 1);
lcd.print("PWM "); 
lcd.print(PWM_DUTY);

if ( PWM_DUTY == 0 ) 
  {
  digitalWrite(LED,LOW);
  lcd.clear();
  lcd.print("Press START");
  }

} 

 

delay(300); //debounce
}  
 

analogWrite(MOSFET_Q4,PWM_DUTY);
}


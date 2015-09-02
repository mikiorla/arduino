

#define SENSORLEVEL 30
#define PERIMETER 0.474

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

int sensor1 = A0; //connected to analog 0
//int sensor2= A1; // connected to analog 1
//int sensor3= A2; // connected to analog 2

int ledPin = 9;    // LED connected to digital pin 9
int sigLed = 8;
int cnt,i=0;
void setup()
{
 lcd.begin(16, 2);
 lcd.print("Counter:");
 lcd.setCursor(8, 0); //col,row
 lcd.print("-");
 Serial.begin(9600);
 pinMode(sigLed, OUTPUT);
}
int newState;
int reading;
int lastState = LOW;   // the previous reading from the input pin

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 20;    // the debounce time; increase if the output flickers

void loop(){
  analogWrite(ledPin, 255);         
  int sensorValue = analogRead(sensor1);  
  Serial.println( sensorValue);
  if (sensorValue < SENSORLEVEL) 
    reading = LOW;
  else
    reading = HIGH; //when there is no detection

  if (reading != lastState)
  {
    // reset the debouncing timer 
    lastDebounceTime = millis(); 
  }
   
  if ((millis()-lastDebounceTime) > debounceDelay) 
  {
    if (reading != newState) 
    {
      newState = reading;
      Serial.println( sensorValue);
      if (newState == LOW)
      {
        i++;
        //Serial.print("Counter:");
        //Serial.println(i);
        lcd.setCursor(8, 0); //col,row
        lcd.print(i);
        lcd.setCursor(0, 1);
        lcd.print("Meters:");
        lcd.print(i*PERIMETER);
        Serial.print(i);
        Serial.println( "DETECTED!");
      }  
    }
  }
  lastState = reading;
}



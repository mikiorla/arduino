

#define senzorLevel 400

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

 
const int UP_pin = 13;
const int NEXT_pin = 12;
int number[10] = {0,1,2,3,4,5,6,7,8,9};

int num_count = 0;
int pos_count=8;

int NEXT_state,UP_state;
int previus_NEXT = LOW;
int previus_UP = LOW;


long lastUPdTime = 0;
long lastNEXTdTime = 0; 
long lastDebounceTime = 0;  
long debounceDelay = 100;

int sensor1 = A0; //connected to analog 0
//int sensor2= A1; // connected to analog 1
//int sensor3= A2; // connected to analog 2

//int NEXT = LOW; 
//int UP = LOW;

int ledPin = 9;    // LED connected to digital pin 9
int sigLed = 8;
int i=0;
void setup()
{
 lcd.begin(16, 2);
 lcd.print("Counter:");
 lcd.setCursor(8, 0); //col,row
 lcd.print("----");
 lcd.setCursor(8, 0); //col,row vrati
  Serial.begin(9600);
  pinMode(sigLed, OUTPUT);
  pinMode(UP_pin, INPUT);
  pinMode(NEXT_pin, INPUT);
  Serial.begin(9600);
}

void loop(){
   lcd.blink();
int  NEXT = digitalRead(NEXT_pin);
int  UP = digitalRead(UP_pin);

if (NEXT == HIGH && UP == LOW)
  {delay(500);
      if (NEXT == HIGH && UP == LOW)
      {
              Serial.print(pos_count);
               Serial.print(num_count);
              if (pos_count <= 11) 
              {
              if (num_count==0) {
                                  
                                  lcd.setCursor(pos_count,0);
                                  lcd.print(number[num_count]);
                                  pos_count++;
                                 }
              else { pos_count++;lcd.setCursor(pos_count,0);}
              }
              if (pos_count == 12) {lcd.setCursor(0, 1);lcd.print("Click NEXT to GO!");num_count=0;}       
              if (pos_count == 13) {lcd.setCursor(0, 1);lcd.print("                 ");lcd.setCursor(0, 1);lcd.print("Runnung!");} 
      Serial.print(pos_count);
     }
  }
 
  
   if (UP == HIGH && NEXT == LOW) 
     {delay(200);
     if (UP == HIGH && NEXT == LOW) 
    {
      
       lcd.setCursor(pos_count, 0);
       lcd.print(number[num_count]);
       num_count++;
       if(num_count == 10) {num_count =0;}           
    }
     }
  
  
  if (NEXT == HIGH && UP == HIGH )
delay(1000);
  if (NEXT == HIGH && UP == HIGH )
  {
  lcd.clear();
  lcd.print("Counter:");
  lcd.setCursor(8, 0); //col,row
  lcd.print("----");
  lcd.setCursor(8, 0); //col,row vrati
  }
  
  
  
   

 
  /*
  lastNEXTstate
  

  //insert: if shaft is turning do measurement else sleep
  analogWrite(ledPin, 255);         
  int sensor1pin = analogRead(sensor1);
  if (sensor1pin < senzorLevel) 
   {
     
      i++;
    
      //Serial.print("Counter:");
      //Serial.println(i);
      lcd.setCursor(8, 0); //col,row
      lcd.print(i);
      lcd.setCursor(0, 1);
      lcd.print("Meters:");
      lcd.print(i/12.56);
      digitalWrite(sigLed, HIGH);
      delay(250);
      digitalWrite(sigLed, LOW);
      
     Serial.println( sensor1pin);
      
      }
    
*/
  
}

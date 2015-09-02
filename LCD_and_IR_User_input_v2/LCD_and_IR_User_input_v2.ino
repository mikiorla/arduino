

#define senzorLevel 400

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

 
const int UP_pin = 13;
const int NEXT_pin = 12;
int number[10] = {0,1,2,3,4,5,6,7,8,9};

int num_count = 1;
int pos_count=8;

int NEXT,UP;
int previus_NEXT = LOW;
int previus_UP = LOW;


long lastUPdTime = 0;
long lastNEXTdTime = 0; 
long lastDebounceTime = 0;  
long debounceDelay = 100;
long holdDelay = 3000;

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
  
readButons();  
int  readingNEXT = digitalRead(NEXT_pin);
int  readingUP = digitalRead(UP_pin);

  if (readingNEXT == HIGH && (millis() - lastNEXTdTime > holdDelay))
  {
    lcd.clear();
   lcd.print("Counter:");
   lcd.setCursor(8, 0); //col,row
   lcd.print("----");
   lcd.setCursor(8, 0); //col,row vrati
  }
  
//  if (readingNEXT == HIGH && previus_NEXT == LOW && millis() - lastNEXTdTime > debounceDelay)  
  if (readingNEXT == HIGH && millis() - lastNEXTdTime > debounceDelay)  
  {           
       if (pos_count <= 11) 
         {              
          num_count=0; 
          lcd.print(number[num_count]);
          num_count++;
          lcd.setCursor(pos_count,0); //sledeci broj
         }
     pos_count++;              
    lastNEXTdTime = millis();
  }

 

  // if (readingUP == HIGH && previus_UP == LOW && millis() - lastUPdTime > debounceDelay) 
   if (readingUP == HIGH && millis() - lastUPdTime > debounceDelay) 
     {
                  
             lcd.setCursor(pos_count, 0);
             lcd.print(number[num_count]);
             
             num_count++;
             if(num_count == 10) {num_count =0;}           
             lastUPdTime = millis();
     } 
   

     
  if (pos_count == 12) {lcd.setCursor(0, 1);lcd.print("Click NEXT to GO!");}       
  if (pos_count == 13) {lcd.setCursor(0, 1);lcd.print("                 ");lcd.setCursor(0, 1);lcd.print("Runnung!");pos_count = 8;}  

 previus_NEXT = readingNEXT;  
 previus_UP = readingUP;   
}

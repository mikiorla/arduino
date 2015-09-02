

#define senzorLevel 400

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

 
const int buttonPinUP = 13;
const int buttonPinNEXT = 12;
int number[10] = {0,1,2,3,4,5,6,7,8,9};

int num_count = 1;
int pos_count=8;

int NEXT,UP;
int previus_NEXT = LOW;
int previus_UP = LOW;

//*************
int lastButtonPushed = 0;

int lastButtonUPState = LOW;   // the previous reading from the Enter input pin
int lastButtonNEXTState = LOW;   // the previous reading from the Esc input pin



long lastUPDebounceTime = 0;  // the last time the output pin was toggled
long lastNEXTDebounceTime = 0;  // the last time the output pin was toggled

long debounceDelay = 500;    // the debounce time
//*************************




long lastUPdTime = 0;
long lastNEXTdTime = 0; 
long lastDebounceTime = 0;  
//long debounceDelay = 100;
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
  pinMode(buttonPinUP, INPUT);
  pinMode(buttonPinNEXT, INPUT);
  Serial.begin(9600);
}

void loop()
{

readButtons();  

if (lastButtonPushed=buttonPinNEXT)
{
if (pos_count <= 11) 
         {              
          num_count=0; 
          lcd.print(number[num_count]);
          num_count++;
          lcd.setCursor(pos_count,0); //sledeci broj
         }
     pos_count++;
}

if (lastButtonPushed=buttonPinUP)
{
  lcd.setCursor(pos_count, 0);
             lcd.print(number[num_count]);
             
             num_count++;
             if(num_count == 10) {num_count =0;}           
             lastUPdTime = millis();
}

 if (pos_count == 12) {lcd.setCursor(0, 1);lcd.print("Click NEXT to GO!");}       
  if (pos_count == 13) {lcd.setCursor(0, 1);lcd.print("                 ");lcd.setCursor(0, 1);lcd.print("Runnung!");pos_count = 8;}


}



void readButtons()
{  //read buttons status
  int reading;
  int buttonUPState=LOW;             // the current reading from the Enter input pin
  int buttonNEXTState=LOW;             // the current reading from the input pin

  //UP button
                  // read the state of the switch into a local variable:
                  reading = digitalRead(buttonPinUP);

                  // check to see if you just pressed the enter button 
                  // (i.e. the input went from LOW to HIGH),  and you've waited 
                  // long enough since the last press to ignore any noise:  
                
                  // If the switch changed, due to noise or pressing:
                  if (reading != lastButtonUPState) {
                    // reset the debouncing timer
                    lastUPDebounceTime = millis();
                  } 
                  
                  if ((millis() - lastUPDebounceTime) > debounceDelay) {
                    // whatever the reading is at, it's been there for longer
                    // than the debounce delay, so take it as the actual current state:
                    buttonUPState=reading;
                    lastUPDebounceTime=millis();
                  }
                  
                  // save the reading.  Next time through the loop,
                  // it'll be the lastButtonState:
                  lastButtonUPState = reading;
                  

    //NEXT button               
                  // read the state of the switch into a local variable:
                  reading = digitalRead(buttonPinNEXT);

                  // check to see if you just pressed the Down button 
                  // (i.e. the input went from LOW to HIGH),  and you've waited 
                  // long enough since the last press to ignore any noise:  
                
                  // If the switch changed, due to noise or pressing:
                  if (reading != lastButtonNEXTState) {
                    // reset the debouncing timer
                    lastNEXTDebounceTime = millis();
                  } 
                  
                  if ((millis() - lastNEXTDebounceTime) > debounceDelay) {
                    // whatever the reading is at, it's been there for longer
                    // than the debounce delay, so take it as the actual current state:
                    buttonNEXTState = reading;
                    lastNEXTDebounceTime=millis();
                  }
                  
                  // save the reading.  Next time through the loop,
                  // it'll be the lastButtonState:
                  lastButtonNEXTState = reading; 
                  
                     
                     //records which button has been pressed
                  if (buttonUPState==HIGH){
                    lastButtonPushed=buttonPinUP;

                  }else if(buttonNEXTState==HIGH){
                    lastButtonPushed=buttonPinNEXT;

                  }else{
                    lastButtonPushed=0;
                  }                  
}



/*
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
}//void
*/

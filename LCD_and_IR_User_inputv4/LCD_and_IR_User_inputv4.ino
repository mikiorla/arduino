

#define senzorLevel 400

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

 
const int UP_pin = 13;
const int NEXT_pin = 12;
int number[10] = {0,1,2,3,4,5,6,7,8,9};
//uint16_t stored_digits[4];
int stored_digits[4];

int Reading(void);

int num_count = 0;
int pos_count=8;
int next_count = 0;
int NEXT_state,UP_state;
int previus_NEXT = LOW;
int previus_UP = LOW;

int meters=0;

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
  pinMode(sigLed, OUTPUT);
  pinMode(UP_pin, INPUT);
  pinMode(NEXT_pin, INPUT);
  
 // lcd.blink();
}

void loop()
{

  Reading();  

}


void StartCounting (void)
{
lcd.clear();
lcd.print("BROJANJE!");
delay(2000);

//**************** Povratak na citanje...
lcd.clear();
lcd.print("Counter:");
lcd.setCursor(8, 0); //col,row
lcd.print("----");
lcd.setCursor(8, 0); //col,row vrati

pos_count=8; // resetuj Gvarijable; mogu biti i samo u f-ji Reading();
num_count=0;
next_count=0;
meters = 0;
stored_digits[4]=0;
Reading();
//***************

}




int Reading(void) 
{
  
int  NEXT = digitalRead(NEXT_pin);
int  UP = digitalRead(UP_pin);

if (NEXT == HIGH && UP == LOW) //********* NEXT ************
  {delay(500);
      if (NEXT == HIGH && UP == LOW)
      {
      //if (next_count == 4) {next_count=0};
             if (pos_count <= 11) 
              {
                  if (num_count==0) {    //nema cifre
                                     lcd.setCursor(pos_count,0);
                                     lcd.print(number[num_count]);
                                     stored_digits[next_count]=num_count; //cifra1
                                     pos_count++;
                                     }
                  else {
                        pos_count++;
                        lcd.setCursor(pos_count,0);
                        stored_digits[next_count]=(num_count-1); //cifra1
                        num_count=0;
                        }Serial.print(pos_count);
              }
              if (pos_count == 12) {lcd.setCursor(0, 1);lcd.print("Click NEXT to GO!");num_count=0;pos_count++;}       
              else if (pos_count == 13)
                     {lcd.setCursor(0, 1);
                     lcd.print("                 ");
                     lcd.setCursor(0, 1);
                     lcd.print("Runnung!");
                     meters = stored_digits[0]*1000 + stored_digits[1]*100 + stored_digits[2]*10 + stored_digits[3]*1 ;
                     lcd.print(meters);  
                     delay(3000);
                     StartCounting();
                   } 
     next_count++;  
     } 
  }
 
  
   if (UP == HIGH && NEXT == LOW) //************** UP *************
     {delay(200);
         if (UP == HIGH && NEXT == LOW) 
            { 
               if (pos_count < 12)
               {
               lcd.setCursor(pos_count, 0);
               lcd.print(number[num_count]);
               num_count++;
               if(num_count == 10) {num_count =0;}           
               }
            }
     }
  
  
  if (NEXT == HIGH && UP == HIGH ) //**************** Reset LCD ************
  {delay(1000);
      if (NEXT == HIGH && UP == HIGH )
      {
        lcd.clear();
        lcd.blink();
        lcd.print("Counter:");
        lcd.setCursor(8, 0); //col,row
        lcd.print("----");
        lcd.setCursor(8, 0); //col,row vrati
        pos_count=8;
        num_count=0;
        next_count=0;
        stored_digits[4]=0;
      }
  }
  
  
 
}

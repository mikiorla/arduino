

#define senzorLevel 400

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// initialize the library with the numbers of the interface pins

 
const int UP_pin = 13;
const int NEXT_pin = 12;

//uint16_t stored_digits[4];
int stored_digits[4];

int Reading(void);

int num_count = 0;
int pos_count=8;
int next_count = 0;

int meters=0;

int sensor1 = A0; //connected to analog 0

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
  lcd.blink();
  
  Serial.begin(9600);
}

void loop()
{
  Reading();  
}

void StartCounting (int meters)
{
lcd.clear();
lcd.print("rwCounting(");
lcd.print(meters);
lcd.print(")");
delay(2000);
int temp = meters;
//neka akcija
while (meters != 0)
    {
     Serial.print(meters);
     Serial.println("\r");
    lcd.setCursor(5,1);
    lcd.print(meters);
    meters--;
    delay(250);
    }
    

//**************** Povratak na citanje...
lcd.clear();
lcd.blink();
lcd.print("Counter:");
lcd.setCursor(8, 0); //col,row
lcd.print("----");
lcd.setCursor(8, 0); //col,row vrati

pos_count=8; // resetuj Gl.varijable; mogu biti i samo u f-ji Reading();
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
        if (num_count != 0)  // zabrani NEXT na sledecu cifru ako nije selektovan makar 0;
            {      
              if (pos_count < 12)
                  {
                  stored_digits[next_count] = (num_count-1); // pre prebacivanja na sledecu cifru sacuvaj je...
                  Serial.println(num_count);
                  Serial.print("Sacuvana cifra:");
                  Serial.println(stored_digits[next_count]);
                                    
                  pos_count++;  
                  lcd.setCursor(pos_count,0);
                  num_count=0; 
                  next_count++;
                  }
              if (pos_count == 12)
                {
                  pos_count++;
                  lcd.setCursor(0, 1);
                  lcd.print("Click NEXT to GO!");
                  num_count=9;
                }
               else if (pos_count == 13)
               {
               lcd.setCursor(0, 1);
               lcd.print("                 ");
               lcd.setCursor(0, 1);
               lcd.noBlink();
               lcd.print("Counting to ");
               meters = stored_digits[0]*1000 + stored_digits[1]*100 + stored_digits[2]*10 + stored_digits[3]*1 ;
               lcd.print(meters);
               delay(2000);
               StartCounting(meters);
               }
            }
      }
  }
  
   if (UP == HIGH && NEXT == LOW) //************** UP *************
     {delay(200);
         if (UP == HIGH && NEXT == LOW) 
            {
            if (pos_count < 12) // ne dozvoli UP dugme kad se ceka na NEXT tj. UP dozvoljeno samo za 4 cifre/pozicije (sa 11-im poljem)
             { 
              if (num_count == 10){num_count=0;} 
              lcd.print(num_count);
              lcd.setCursor(pos_count,0);
              num_count++;
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
 


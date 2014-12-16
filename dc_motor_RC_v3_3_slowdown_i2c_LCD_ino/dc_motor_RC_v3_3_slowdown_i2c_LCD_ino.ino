


// TCCR1B = TCCR1B & 0b11111000 | 0x01 // http://playground.arduino.cc/Code/PwmFrequency
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

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//PCF8574AP ima adresu 0x38 (0111000 - A0 A1 A2 na GND)

LiquidCrystal_I2C lcd(0x38,16,2);  // set the LCD address to 0x38 for a 16 chars and 2 line display


//#define SendPlot 

int Enter_meters(void);
void Autonomous(void);
void Manual(void);
int Reading(void);

//Motor1 is atthached to (LEFT controller)
const int M1_Q1= 10;
const int M1_Q2 = 13;
const int M1_Q3= 12;
const int M1_Q4= 11;

//Motor2 is attached to (RIGHT controller)
const int M2_Q1= 9;
const int M2_Q2= 6;
const int M2_Q3 = 7;
const int M2_Q4= 3;

//const int LED = 13;
//const int LED_RW = 8;
const int FAN = 2;

const int RC_FW = A1;
const int RC_RW = A0;
const int RC_LEFT = A3;
const int RC_RIGHT = A2;


//int clear_lcd = 1;
int GO_FW = 0;
int GO_RW = 0;
int GO_LEFT = 0;
int GO_RIGHT = 0;
unsigned int PWM_DUTY_FW,PWM_DUTY_RW = 0;
int PWM_DIFF = 65;
int FW_RW_DELAY = 2;
int PWM_FW_lcd,PWM_RW_lcd;

char mode='M';
int m=0;
int pos_count=13;
int next_count = 0;
int num_count = 0;
int meters=0;
int fw_meters=0;
int fw,rw = 0;
int auto_fw_once =1;



long FAN_timeout = 5000; 
long AUTO_FW_TIME_CHECK;
long previous_milis = 0;
long i_fw,i_fw_temp,lcd_one_time= 0;

void setup()
{
  Serial.begin(9600);
  //lcd.begin(16, 2);
  lcd.init();
  delay(100);
  lcd.print("Select mode:");
  lcd.setCursor(12, 0); //col,row
  lcd.print("M");
  lcd.setCursor(0, 1);
  lcd.print("A-Auto M-manual ");
  lcd.setCursor(12, 0); //col,row

  pinMode(RC_FW, INPUT);
  pinMode(RC_RW, INPUT);
  pinMode(RC_LEFT, INPUT);
  pinMode(RC_RIGHT, INPUT);
  
  pinMode(M1_Q1, OUTPUT);
  pinMode(M1_Q2, OUTPUT);
  pinMode(M1_Q3, OUTPUT);
  pinMode(M1_Q4, OUTPUT);

  pinMode(M2_Q1, OUTPUT);
  pinMode(M2_Q2, OUTPUT);
  pinMode(M2_Q3, OUTPUT);
  pinMode(M2_Q4, OUTPUT);
  
  pinMode(FAN, OUTPUT);

  TCCR1B = TCCR1B & 0b11111000 | 0x01 ;
  TCCR2B = TCCR2B & 0b11111000 | 0x01 ;
  delay(1000);

} //**********************************************end SETUP

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

#ifdef SendPlot
  sendPlotData("Forward",GO_FW);
  sendPlotData("Reverse",GO_RW );
  sendPlotData("Left",GO_LEFT);
  sendPlotData("Right",GO_RIGHT);
#endif

 if ((GO_FW > 900) & (GO_RW < 100))//UP
            {
            delay(20);
            if ((GO_FW > 900) & (GO_RW < 100))//UP
               {
                lcd.setCursor(12, 0); //col,row
                lcd.print("M");
                lcd.setCursor(12, 0);
                mode='M';
               }
            }           

if ((GO_RW > 900) & (GO_FW < 100)) //
                {
                 delay(20);
                 if ((GO_RW > 900) & (GO_FW < 100)) //DW
                   {
                   lcd.setCursor(12, 0); //col,row
                   lcd.print("A");
                   lcd.setCursor(12, 0);
                   mode='A';               
                   }   
                }            
                               
   
  
  


if ((GO_LEFT < 100) & (GO_RIGHT > 900)) ////Right button to select and start
{
delay(20);
if ((GO_LEFT < 100) & (GO_RIGHT > 900))
{ 

  switch(mode)
                                    {
                                    case 'A':
                                      lcd.setCursor(0, 1);
                                      lcd.print("AUTO selected   "); 
                                      delay(1000);
                                      lcd.clear();
                                      lcd.setCursor(0, 0);
                                      lcd.print("AUTOnomous mode");
                                      lcd.setCursor(0, 1);
                                      lcd.print("Redirecting...");
                                      lcd.setCursor(13,1);
                                      delay(2000);
                                      Autonomous();
                                      //Enter_meters();
                                      break;              
                                    
                                    case 'M':  
                                      lcd.setCursor(0, 1);
                                      lcd.print("MANUAL selected ");
                                      delay(2000);
                                      lcd.clear();
                                      lcd.setCursor(0, 0);
                                      lcd.print("MANUAL mode");
                                      lcd.setCursor(0, 1);
                                      lcd.print("Waiting input...");
                                      lcd.setCursor(13,1); 
                                      Manual();
                                       delay(2000);
                                     break;            
                                    } 
                                    
  
 }
  
}


digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M1_Q4,LOW);

digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);
digitalWrite(M2_Q4,LOW);
}  

 
//  ******************************************** MANUAL ********************************************
void Manual(void)                                     
{  
while(1)
{

GO_FW = analogRead(RC_FW); 
GO_RW = analogRead(RC_RW); 
GO_LEFT = analogRead(RC_LEFT); 
GO_RIGHT = analogRead(RC_RIGHT); 

      //**************************************************** FW ************************************
      if ((GO_FW > 1000) && (GO_RW < 100))//napred
            {
            delay(20);
            while ((GO_FW > 1000) && (GO_RW < 100))//napred
              {
              
             
              digitalWrite(FAN,HIGH);
                
              digitalWrite(M1_Q3,LOW);
              digitalWrite(M2_Q3,LOW);
              digitalWrite(M1_Q4,LOW);
              digitalWrite(M2_Q4,LOW);
                       
              if(fw==0)
              {
                for(;PWM_DUTY_FW<255;PWM_DUTY_FW++)
                 {
                  
                  analogWrite(M2_Q1,PWM_DUTY_FW);
                  analogWrite(M1_Q1,PWM_DUTY_FW);
                  digitalWrite(M2_Q2,HIGH); 
                  digitalWrite(M1_Q2,HIGH); 
                  delay(FW_RW_DELAY);
                  }
               fw=1;
              analogWrite(M2_Q1,PWM_DUTY_FW);
              analogWrite(M1_Q1,PWM_DUTY_FW-PWM_DIFF);
              }
      
              
             if(auto_fw_once){ AUTO_FW_TIME_CHECK = millis(); auto_fw_once=0;} //enter once to take real time in miliseconds
             if (millis() - AUTO_FW_TIME_CHECK > 2000)
                {
                  while (!(  (GO_RW > 900) && (GO_FW < 100)  ))//dokle god se nestisne RW idi samo napred
                      {
                     /* digitalWrite(M1_Q2,HIGH); 
                      digitalWrite(M2_Q2,HIGH); 
                      analogWrite(M1_Q1,PWM_DUTY_FW);
                      analogWrite(M2_Q1,PWM_DUTY_FW);*/
                      GO_RW = analogRead(RC_RW); 
                      GO_LEFT = analogRead(RC_LEFT); 
                      GO_FW = analogRead(RC_FW);
                      GO_RIGHT = analogRead(RC_RIGHT); 
                      }
                     delay(500);
                  
                
                }             
         
              previous_milis = millis();     
         
              GO_RW = analogRead(RC_RW); 
              GO_LEFT = analogRead(RC_LEFT); 
              GO_FW = analogRead(RC_FW);
              GO_RIGHT = analogRead(RC_RIGHT); 
              
              #ifdef SendPlot
                sendPlotData("Forward",GO_FW);
                sendPlotData("Reverse",GO_RW );
                sendPlotData("Left",GO_LEFT);
                sendPlotData("Right",GO_RIGHT);
              #endif
              
            }
             lcd_one_time=1;             
            }
        //END**************************************************** FW ************************************  
      
      if (fw==1) //************************************************slow down from FW
        {
            for(;PWM_DUTY_FW > 0;PWM_DUTY_FW--)
            {
             analogWrite(M1_Q1,PWM_DUTY_FW-PWM_DIFF);
             analogWrite(M2_Q1,PWM_DUTY_FW);
             digitalWrite(M1_Q2,HIGH); //M1
             digitalWrite(M2_Q2,HIGH); //M2
             delay(FW_RW_DELAY);
            }
           PWM_DUTY_FW=0;
           fw=0;
           digitalWrite(M1_Q2,LOW); 
           digitalWrite(M2_Q2,LOW); 
//           analogWrite(M1_Q1,PWM_DUTY_FW);
//           analogWrite(M2_Q1,PWM_DUTY_FW);
           analogWrite(M1_Q1,0);
           analogWrite(M2_Q1,0);
         
       } //************************************************slow down from FW

        //**************************************************** RW ***************************************
        if  ((GO_RW > 1000) && (GO_FW < 100)) //nazad
                {
                delay(20);
                while ((GO_RW > 1000) && (GO_FW < 100)) //nazad
                  {
                  
                  digitalWrite(FAN,HIGH);
                  
                  digitalWrite(M1_Q2,LOW); 
                  digitalWrite(M2_Q2,LOW);   
                  digitalWrite(M1_Q1,LOW); 
                  digitalWrite(M2_Q1,LOW); 
                 
              if(rw==0)
              {
                for(;PWM_DUTY_RW <255;PWM_DUTY_RW++)
                 {
                  analogWrite(M2_Q4,PWM_DUTY_RW);
                  analogWrite(M1_Q4,PWM_DUTY_RW);
                  digitalWrite(M2_Q3,HIGH); 
                  digitalWrite(M1_Q3,HIGH);
                                  
                  delay(FW_RW_DELAY);
                  }
               rw=1;
              }
                  
                  analogWrite(M2_Q4,PWM_DUTY_RW);
                  analogWrite(M1_Q4,PWM_DUTY_RW-PWM_DIFF);

                  previous_milis = millis();  
                 
                  GO_RW = analogRead(RC_RW); 
                  GO_LEFT = analogRead(RC_LEFT); 
                  GO_FW = analogRead(RC_FW);
                  GO_RIGHT = analogRead(RC_RIGHT); 
                
                 
                  #ifdef SendPlot
                    sendPlotData("Forward",GO_FW);
                    sendPlotData("Reverse",GO_RW );
                    sendPlotData("Left",GO_LEFT);
                    sendPlotData("Right",GO_RIGHT);
                  #endif
                   
                }
                 lcd_one_time=1 ;
                } 
        //END**************************************************** RW ***************************************  
        
        if (rw==1) //************************************************slow down from RW
          {    
            PWM_RW_lcd=PWM_DUTY_RW;
              for(;PWM_DUTY_RW > 0;PWM_DUTY_RW--)
              {
               digitalWrite(M1_Q3,HIGH); 
               digitalWrite(M2_Q3,HIGH); 
               analogWrite(M2_Q4,PWM_DUTY_RW);
               analogWrite(M1_Q4,PWM_DUTY_RW-PWM_DIFF);
               delay(FW_RW_DELAY);
              }
             PWM_DUTY_RW=0;
             rw=0;  
             digitalWrite(M1_Q3,LOW); 
             digitalWrite(M2_Q3,LOW); 
//             analogWrite(M1_Q4,PWM_DUTY_RW);
//             analogWrite(M2_Q4,PWM_DUTY_RW);
             analogWrite(M1_Q4,0);
             analogWrite(M2_Q4,0);
             

           
         } //END************************************************slow down from RW
        
        
        
        
        

//**************************************************** RIGHT***************************************

if ((GO_RIGHT > 1000) && (GO_LEFT < 100)) 
//if ((GO_RIGHT > 1000)) 
{delay(20);
    while ((GO_RIGHT > 1000) && (GO_LEFT < 100)) 
    {
    digitalWrite(FAN,HIGH);
    //levi motor napred,desni polako ili nazad
//    digitalWrite(M1_Q2,HIGH); //M1  
//    analogWrite(M1_Q1,150);
//    digitalWrite(M1_Q1,HIGH); //M1
              
              digitalWrite(M2_Q1,LOW);    
              digitalWrite(M2_Q2,LOW); 
              digitalWrite(M1_Q3,LOW);
              digitalWrite(M1_Q4,LOW);

              analogWrite(M2_Q4,100); //M2 RW
              analogWrite(M1_Q1,130); //M1 FW

              
              digitalWrite(M2_Q3,HIGH); //M2 RW
              digitalWrite(M1_Q2,HIGH); //M1 FW
              
              
            
     GO_RW = analogRead(RC_RW); 
     GO_LEFT = analogRead(RC_LEFT); 
     GO_FW = analogRead(RC_FW);
     GO_RIGHT = analogRead(RC_RIGHT); 
     previous_milis = millis();  
     
     #ifdef SendPlot
     sendPlotData("Forward",GO_FW);
     sendPlotData("Reverse",GO_RW );
     sendPlotData("Left",GO_LEFT);
     sendPlotData("Right",GO_RIGHT);
     #endif
     
     
     
    }
}//END **************************************************** RIGHT ***************************************


//**************************************************** LEFT ***************************************
if ((GO_LEFT > 1000) && (GO_RIGHT < 100)) 
{
  delay(20);
  while ((GO_LEFT > 1000) && (GO_RIGHT < 100))  
  {
//desni motor napred,levi polako ili nazad
  digitalWrite(FAN,HIGH);
    //levi motor napred,desni polako ili nazad
//    digitalWrite(M1_Q2,HIGH); //M1  
//    analogWrite(M1_Q1,150);
//    digitalWrite(M1_Q1,HIGH); //M1
              
              
              digitalWrite(M1_Q1,LOW); 
              digitalWrite(M1_Q2,LOW); 
              digitalWrite(M2_Q4,LOW);
              digitalWrite(M2_Q3,LOW);
              
              analogWrite(M2_Q1,130);  //M2 FW  
              analogWrite(M1_Q4,100); //M1 RW
              
              digitalWrite(M2_Q2,HIGH); 
              digitalWrite(M1_Q3,HIGH);
              
              

            
            
             /* analogWrite(M1_Q1,0);
              analogWrite(M2_Q1,200);
              digitalWrite(M1_Q2,LOW); //M1
              digitalWrite(M2_Q2,HIGH); //M2
              */



     GO_RW = analogRead(RC_RW); 
     GO_LEFT = analogRead(RC_LEFT); 
     GO_FW = analogRead(RC_FW);
     GO_RIGHT = analogRead(RC_RIGHT); 
     previous_milis = millis();  

      #ifdef SendPlot
      sendPlotData("Forward",GO_FW);
      sendPlotData("Reverse",GO_RW );
      sendPlotData("Left",GO_LEFT);
      sendPlotData("Right",GO_RIGHT);
      #endif



  }
}//END **************************************************** LEFT ***************************************

 #ifdef SendPlot
   sendPlotData("Forward",GO_FW);
   sendPlotData("Reverse",GO_RW );
   sendPlotData("Left",GO_LEFT);
   sendPlotData("Right",GO_RIGHT);
 #endif



if(lcd_one_time)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("last PWM_FW:");
lcd.print(PWM_DUTY_FW);
lcd.setCursor(0, 1);
lcd.print("last PWM_RW:");
lcd.print(PWM_RW_lcd);
lcd_one_time=0;
}



digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);

digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);
    
if(millis() - previous_milis > FAN_timeout) {digitalWrite(FAN,LOW);}
auto_fw_once = 1;
} // end while(1)
}//  ******************************************** end MANUAL ********************************************

/*
int Enter_meters() //}//*************************************************** Enter_meters(); **********************
{
       m=0;
      num_count=0;
      //int  NEXT = digitalRead(NEXT_pin);
//      int  UP = digitalRead(UP_pin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("AUTO forward");
      lcd.setCursor(0, 1);
      lcd.print("Enter meters:---");
       lcd.setCursor(13, 1);
      lcd.blink();
      //**************
      while (m<3)
      {
        
      int  NEXT = digitalRead(NEXT_pin);
      int  UP = digitalRead(UP_pin);
       if (NEXT == HIGH && UP == LOW) //********* NEXT Digit ************
                                {
                                  delay(250);
                                  if (NEXT == HIGH && UP == LOW)
                                 {
                                      if (num_count != 0)  // zabrani NEXT na sledecu cifru ako nije selektovan makar 0;
                                          {      
                                            if (pos_count < 16)
                                                {
                                                stored_digits[next_count] = (num_count-1); // pre prebacivanja na sledecu cifru sacuvaj je...
                                                pos_count++;  
                                                lcd.setCursor(pos_count,1);
                                                num_count=0; 
                                                next_count++;
                                                m++;
                                                }
                                            if (pos_count == 16)
                                              {
                                                pos_count++;
                                                lcd.setCursor(0, 1);
                                                lcd.print("click NEXT to go!");
                                                num_count=9;
                                                m=0;
                                              }
                                             else if (pos_count == 17)
                                             {
                                             lcd.setCursor(0, 1);
                                             lcd.print("                 ");
                                             lcd.setCursor(0, 1);
                                             lcd.noBlink();
                                             lcd.print("going ");
                                             meters = stored_digits[0]*100 + stored_digits[1]*10 + stored_digits[2]*1 ;
                                             lcd.print(meters);
                                             lcd.print(" meters fw");
                                             delay(2000);
                                            return meters;
                                            
                                             }
                                          }
                                    }
                                }
         
             
      if (NEXT == LOW && UP == HIGH) //*********** Enter digits ************
                { delay(250);
                if (NEXT == LOW && UP == HIGH)
                {
                if (pos_count < 16) // ne dozvoli UP dugme kad se ceka na NEXT tj. UP dozvoljeno samo za 3 cifre/pozicije (sa 11-im poljem)
                         { 
                          if (num_count == 10){num_count=0;} 
                          lcd.print(num_count);
                          lcd.setCursor(pos_count,1);
                          num_count++;
                         }
                }
                }
                
      } //while(m<3)
  
  
      
}//**************************************************************end Enter_meters(); **********************

*/

void Autonomous(void)
{

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("A-MODE STARTED");

while(1){

//ALL OFF
digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);

digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);
delay(500);

digitalWrite(FAN,HIGH);
    
//*********************************************************************************FW

if(fw==0)
  {
    for(;PWM_DUTY_FW<255;PWM_DUTY_FW++)
     {
      
       analogWrite(M2_Q1,PWM_DUTY_FW);
       analogWrite(M1_Q1,PWM_DUTY_FW);
       digitalWrite(M2_Q2,HIGH); 
       digitalWrite(M1_Q2,HIGH); 
  
      delay(FW_RW_DELAY);
      }
   fw=1;
    analogWrite(M2_Q1,PWM_DUTY_FW);
    analogWrite(M1_Q1,PWM_DUTY_FW);
}

delay(1000);

if (fw==1) //************************************************slow down from FW
        {
            for(;PWM_DUTY_FW > 0;PWM_DUTY_FW--)
            {
             analogWrite(M1_Q1,PWM_DUTY_FW);
             analogWrite(M2_Q1,PWM_DUTY_FW);
             digitalWrite(M1_Q2,HIGH); //M1
             digitalWrite(M2_Q2,HIGH); //M2
             delay(FW_RW_DELAY);
            }
           PWM_DUTY_FW=0;
           fw=0;
           digitalWrite(M1_Q2,LOW); 
           digitalWrite(M2_Q2,LOW); 
           analogWrite(M1_Q1,PWM_DUTY_FW);
           analogWrite(M2_Q1,PWM_DUTY_FW);
       } //************************************************slow down from FW
//END *********************************************************************************FW



digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);

digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);
delay(500);

//**************************************RIGHT

      digitalWrite(M2_Q1,LOW);    
      digitalWrite(M1_Q4,LOW);      
      digitalWrite(M2_Q2,LOW); 
      digitalWrite(M1_Q3,LOW);
      
      digitalWrite(M1_Q2,HIGH); //M1 FW
      digitalWrite(M2_Q3,HIGH); //M2 RW

      analogWrite(M2_Q4,100); //M2 RW
      analogWrite(M1_Q1,150); //M1 FW

      
      delay(1200);
//END**************************************RIGHT

//ALL OFF
digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);
  
digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);
delay(500);



//*********************************************************************************FW
          
if(fw==0)
  {
    for(;PWM_DUTY_FW<255;PWM_DUTY_FW++)
     {
      
      analogWrite(M2_Q1,PWM_DUTY_FW); 
      analogWrite(M1_Q1,PWM_DUTY_FW);

      digitalWrite(M2_Q2,HIGH); 
      digitalWrite(M1_Q2,HIGH); 
      
      delay(FW_RW_DELAY);
      }
   fw=1;

   analogWrite(M2_Q1,PWM_DUTY_FW);
   analogWrite(M1_Q1,PWM_DUTY_FW);
}

delay(1000);

if (fw==1) //************************************************slow down from FW
        {
            for(;PWM_DUTY_FW > 0;PWM_DUTY_FW--)
            {
             analogWrite(M1_Q1,PWM_DUTY_FW);
             analogWrite(M2_Q1,PWM_DUTY_FW);
             digitalWrite(M1_Q2,HIGH); //M1
             digitalWrite(M2_Q2,HIGH); //M2
             delay(FW_RW_DELAY);
            }
           PWM_DUTY_FW=0;
           fw=0;
           analogWrite(M1_Q1,PWM_DUTY_FW);
           analogWrite(M2_Q1,PWM_DUTY_FW);
           digitalWrite(M1_Q2,LOW); 
           digitalWrite(M2_Q2,LOW); 
           
       } //************************************************slow down from FW
//END *********************************************************************************FW

//ALL OFF
digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);

digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);
delay(500);


//**************************************RIGHT
      digitalWrite(M2_Q1,LOW);    
      digitalWrite(M1_Q4,LOW);      
      digitalWrite(M2_Q2,LOW); 
      digitalWrite(M1_Q3,LOW);
      
      digitalWrite(M1_Q2,HIGH); //M1 FW
      digitalWrite(M2_Q3,HIGH); //M2 RW

      analogWrite(M1_Q1,150); //M1 FW
      analogWrite(M2_Q4,100); //M2 RW
      
      delay(1200);
//END**************************************RIGHT
  
} // end while(1)
} // end Autonomous



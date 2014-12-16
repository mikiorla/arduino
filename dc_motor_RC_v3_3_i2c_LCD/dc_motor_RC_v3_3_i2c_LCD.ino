


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


#define SendPlot 

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
const int M2_Q4= 8;

const int LED = 13;
//const int LED_RW = 8;
const int FAN = 3;

const int RC_FW = A1;
const int RC_RW = A0;
const int RC_LEFT = A3;
const int RC_RIGHT = A2;


//int clear_lcd = 1;
int GO_FW = 0;
int GO_RW = 0;
int GO_LEFT = 0;
int GO_RIGHT = 0;
int PWM_DUTY = 0;

char mode='M';
int m=0;
int pos_count=13;
int next_count = 0;
int num_count = 0;
int meters=0;
int fw_meters=0;
int fw=0;
int soft;

long FAN_timeout = 5000; 
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

          GO_RW = digitalRead(RC_RW); 
                  GO_LEFT = digitalRead(RC_LEFT); 
                  GO_FW = digitalRead(RC_FW);
                  GO_RIGHT = digitalRead(RC_RIGHT); 

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
                                      delay(2000);
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
              
              //digitalWrite(M1_Q1,HIGH); //PWM pin M1
              //digitalWrite(M2_Q1,HIGH); //PWM pin M2
              
              if(PWM_DUTY==0)
              {
                for(;PWM_DUTY<255;PWM_DUTY++)
                 {
                  analogWrite(M1_Q1,PWM_DUTY);
                  analogWrite(M2_Q1,PWM_DUTY);
                  digitalWrite(M1_Q2,HIGH); //M1
                  digitalWrite(M2_Q2,HIGH); //M2
                  delay(1);
                  }
              }
                           
              analogWrite(M1_Q1,190);
              analogWrite(M2_Q1,255);
              
//              analogWrite(M1_Q1,150);
//              analogWrite(M2_Q1,150);
                
                
              digitalWrite(M1_Q2,HIGH); //M1
              digitalWrite(M2_Q2,HIGH); //M2
              
              /*
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Going Forward");
              */
              previous_milis = millis();     
              //lcd_one_time = 1;
              //i_fw++; 
              //i_fw_temp = i_fw;
                      GO_RW = digitalRead(RC_RW); 
                  GO_LEFT = digitalRead(RC_LEFT); 
                  GO_FW = digitalRead(RC_FW);
                  GO_RIGHT = digitalRead(RC_RIGHT);  
              
              #ifdef SendPlot
                sendPlotData("Forward",GO_FW);
                sendPlotData("Reverse",GO_RW );
                sendPlotData("Left",GO_LEFT);
                sendPlotData("Right",GO_RIGHT);
              #endif
              
              }
              
              fw=1;
            }
        //END**************************************************** FW ************************************  


        //**************************************************** RW ***************************************
        if  ((GO_RW > 900) && (GO_FW < 100)) //nazad
                {
                delay(20);
                while((GO_RW > 900) && (GO_FW < 100)) //nazad
                  {
                  
                  digitalWrite(FAN,HIGH);
                  
                  digitalWrite(M1_Q2,LOW); 
                  digitalWrite(M2_Q2,LOW);   
                  digitalWrite(M1_Q1,LOW); 
                  digitalWrite(M2_Q1,LOW); 
                 
                  
                  digitalWrite(M1_Q3,HIGH);
                  digitalWrite(M2_Q3,HIGH);
                  
                  digitalWrite(M1_Q4,HIGH);
                  digitalWrite(M2_Q4,HIGH);
                  
                  /*
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Reverse");
                  */
                  
                  previous_milis = millis();  
                  lcd_one_time = 1;              
                  GO_RW = digitalRead(RC_RW); 
                  GO_LEFT = digitalRead(RC_LEFT); 
                  GO_FW = digitalRead(RC_FW);
                  GO_RIGHT = digitalRead(RC_RIGHT); 
                
                 
                  #ifdef SendPlot
                    sendPlotData("Forward",GO_FW);
                    sendPlotData("Reverse",GO_RW );
                    sendPlotData("Left",GO_LEFT);
                    sendPlotData("Right",GO_RIGHT);
                  #endif
                  }
                } 
        //END**************************************************** RW ***************************************  

//**************************************************** RIGHT***************************************

//if ((GO_RIGHT > 1000) && (GO_LEFT < 100)) 
if ((GO_RIGHT > 1000)) 
{delay(10);
  if ((GO_RIGHT > 1000)) 
    {
    digitalWrite(FAN,HIGH);
    //levi motor napred,desni polako ili nazad
//    digitalWrite(M1_Q2,HIGH); //M1  
//    analogWrite(M1_Q1,150);
//    digitalWrite(M1_Q1,HIGH); //M1
              digitalWrite(M1_Q3,LOW);
              digitalWrite(M2_Q3,LOW);
              digitalWrite(M1_Q4,LOW);
              digitalWrite(M2_Q4,LOW);
            
            
              analogWrite(M1_Q1,255);
              analogWrite(M2_Q1,255);
              digitalWrite(M1_Q2,HIGH); //M1
              digitalWrite(M2_Q2,HIGH); //M2
              


          GO_RW = digitalRead(RC_RW); 
                  GO_LEFT = digitalRead(RC_LEFT); 
                  GO_FW = digitalRead(RC_FW);
                  GO_RIGHT = digitalRead(RC_RIGHT); 


/*
    // GO_RW = analogRead(RC_RW); 
     GO_LEFT = analogRead(RC_LEFT); 
    // GO_FW = analogRead(RC_FW);
     GO_RIGHT = analogRead(RC_RIGHT); 
     
*/
     previous_milis = millis();  
    }
}//END **************************************************** RIGHT ***************************************


//**************************************************** LEFT ***************************************
if ((GO_LEFT > 900) && (GO_RIGHT < 100)) 
{
  if ((GO_LEFT > 900) && (GO_RIGHT < 100))  
  {
//desni motor napred,levi polako ili nazad
  digitalWrite(FAN,HIGH);
    //levi motor napred,desni polako ili nazad
//    digitalWrite(M1_Q2,HIGH); //M1  
//    analogWrite(M1_Q1,150);
//    digitalWrite(M1_Q1,HIGH); //M1
              digitalWrite(M1_Q3,LOW);
              digitalWrite(M2_Q3,LOW);
              digitalWrite(M1_Q4,LOW);
              digitalWrite(M2_Q4,LOW);
            
            
              analogWrite(M1_Q1,150);
              analogWrite(M2_Q1,150);
              digitalWrite(M1_Q2,HIGH); //M1
              digitalWrite(M2_Q2,HIGH); //M2
              


/*
    // GO_RW = analogRead(RC_RW); 
     GO_LEFT = analogRead(RC_LEFT); 
    // GO_FW = analogRead(RC_FW);
     GO_RIGHT = analogRead(RC_RIGHT); 
     
*/
     previous_milis = millis();  




  }
}//END **************************************************** LEFT ***************************************

 #ifdef SendPlot
   sendPlotData("Forward",GO_FW);
   sendPlotData("Reverse",GO_RW );
   sendPlotData("Left",GO_LEFT);
   sendPlotData("Right",GO_RIGHT);
 #endif
digitalWrite(M1_Q2,LOW);
digitalWrite(M1_Q3,LOW);
digitalWrite(M2_Q2,LOW);
digitalWrite(M2_Q3,LOW);

digitalWrite(M1_Q1,LOW);
digitalWrite(M1_Q4,LOW);
digitalWrite(M2_Q1,LOW);
digitalWrite(M2_Q4,LOW);

/*
if(lcd_one_time)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("i_FW:");
lcd.print(i_fw_temp);
i_fw=0;
lcd_one_time=0;
}
*/



//************************************************slow down
if (fw==1)
  {
      for(;PWM_DUTY > 0;PWM_DUTY--)
      {
       analogWrite(M1_Q1,PWM_DUTY);
       analogWrite(M2_Q1,PWM_DUTY);
       digitalWrite(M1_Q2,HIGH); //M1
       digitalWrite(M2_Q2,HIGH); //M2
       delay(1);
      }
     fw=0;  
 }
//END************************************************slow down 
    
if(millis() - previous_milis > FAN_timeout) {digitalWrite(FAN,LOW);}

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
while(1){
/*
digitalWrite(LED,HIGH);
digitalWrite(LED_RW,LOW);
delay(1000);
digitalWrite(LED,LOW);
digitalWrite(LED_RW,HIGH);
delay(1000);    
*/
lcd.clear();
delay(500);
lcd.setCursor(0, 0);
lcd.print("AUTOnomous mode");
delay(500);

  
} // end while(1)
} // end Autonomous



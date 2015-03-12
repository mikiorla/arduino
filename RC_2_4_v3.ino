



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

//#include <GraphSeries.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//PCF8574AP ima adresu 0x38 (0111000 - A0 A1 A2 na GND)

LiquidCrystal_I2C lcd(0x38,16,2);  // set the LCD address to 0x38 for a 16 chars and 2 line display


//#define SendPlot 
//#define Serial_view_monitor

int Enter_meters(void);
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
const int channel_2 = 5;
const int channel_1 = 4;
const int channel_3 = 8;

int CH1,CH2,CH3;
int CH1_prev,CH2_prev,CH3_prev = 0;

//int clear_lcd = 1;
unsigned int PWM_DUTY_FW,PWM_DUTY_RW = 0;

int PWM_M1 = 140; //****************************************brzina motora M1
//M-diff  = 115
int FW_RW_DELAY = 3;

char mode='M';

int auto_fw_entered = 0;
int auto_fw_once =1;
int menu = 1;


long FAN_timeout = 5000; 
long AUTO_FW_TIME_CHECK;
long previous_milis = 0;

void setup()
{
  Serial.begin(9600);
  //lcd.begin(16, 2);
  lcd.init();
  delay(100);
  pinMode(channel_1, INPUT);
  pinMode(channel_2, INPUT);
  pinMode(channel_3, INPUT);


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
  delay(100);

} //**********************************************end SETUP

void loop()
{

  CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
  CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
  CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1

  if (menu)
  {
    lcd.print("RC_2_4_v3");
    delay(1000);
    lcd.clear();
    lcd.print("Select mode:");
    lcd.setCursor(12, 0); //col,row
    lcd.print("M");
    lcd.setCursor(0, 1);
    lcd.print("S-Signal M-Manual");
    lcd.setCursor(12, 0); //col,row
  }
  menu=0;

if (CH2 < 1300)//FW
  {
    lcd.setCursor(12, 0); //col,row
    lcd.print("M");
    lcd.setCursor(12, 0);
    mode='M';
  }           

  if (CH2 > 1600) //RW
  {
    lcd.setCursor(12, 0); //col,row
    lcd.print("S");
    lcd.setCursor(12, 0);
    mode='S';               
  }     
      
  if (CH1 > 1600) //Right button selected
  {
  switch(mode)
  {
     case 'M':
        { 
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("MANUAL selected ");
            lcd.setCursor(0,1);
            lcd.print("M1 speed:");
            lcd.setCursor(9, 1);
            lcd.print(PWM_M1);
            while (CH1 < 1600) //WHILE Right button NOT selected
              {
              if (CH2 < 1300) { if(PWM_M1 < 200){ PWM_M1+=2;}} 
              if (CH2 > 1600) { if(PWM_M1 != 0){ PWM_M1-=2;}} 
              lcd.setCursor(9, 1);
              lcd.print(PWM_M1);
              CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
              CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
              }
            
            delay(2000);
            Manual();
            break;
        }
    case 'S':
        {
        serial_view();
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

void serial_view()
{
  CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
  CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
  CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1
  
  while (CH3 < 1500)
  {
      if (abs(CH1 - CH1_prev) > 50)
      {
        lcd.print("CH1:");
        lcd.print(CH1);
        CH1_prev = CH1;
      }
      if (abs(CH2 - CH2_prev) > 50)
      {
        lcd.setCursor(8, 0); //col,row
        lcd.print("CH2:"); //Prints the channels value on the serial monitor
        lcd.print(CH2); //Prints the channels value on the serial monitor
        CH2_prev = CH2;
      }
      if (abs(CH3 - CH3_prev) > 50)
      {
        lcd.setCursor(0,1); //col,row
        lcd.print("CH3:"); //Prints the channels value on the serial monitor
        lcd.println (CH3); //Prints the channels value on the serial monitor
        CH3_prev = CH3;
      }
  }
  menu = 1;
  return;

}





//   Q1    Q3
//    | 
//     -M-
//        |
//   Q4    Q2


//  ******************************************** MANUAL ********************************************
void Manual(void)                                     
{
  //Read inputs; 
  CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
  CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
  CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1

  //while(1)
  while(CH1 != 0 && CH2!=0 && CH3 != 0)
//  while(CH1 != 0 && CH2!=0 && CH3 < 1000)
  {

    //**************************************************** FW ************************************
    if (CH2 < 1300)//napred
    {
      while (CH2 < 1300)           
      {
        digitalWrite(FAN,HIGH);

        digitalWrite(M1_Q3,LOW);
        digitalWrite(M2_Q3,LOW);
        digitalWrite(M1_Q4,LOW);
        digitalWrite(M2_Q4,LOW);

      
        digitalWrite(M2_Q2,HIGH); 
        digitalWrite(M1_Q2,HIGH); 
        //digitalWrite(M2_Q1,HIGH);
        analogWrite(M2_Q1,255);
        analogWrite(M1_Q1,PWM_M1);          // PWM M1
        

        // 
        if(auto_fw_once){ 
          AUTO_FW_TIME_CHECK = millis(); 
          auto_fw_once=0;
        } //enter once to take real time in miliseconds
        if (millis() - AUTO_FW_TIME_CHECK > 1000)
        {
          auto_fw_entered = 1;
          while (!(CH2 > 1600) && CH2!=0)//dokle god se nestisne RW idi samo napred ____//while !rw___//****************************AUTO_FW
          {

           //**************************  LEFT in AUTO_FW **********************
            if (CH1 < 1300) 
            {
              delay(10);
              while (CH1 < 1300) 
              {
              
                //levi ugasi
                digitalWrite(M2_Q2,LOW); 

                CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
                CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
 
              }
                 
            }
            //END **************************  LEFT  *******************
            digitalWrite(M2_Q2,HIGH);//levi ponovo upali 

            //**************************  RIGHT  in AUTO_FW **********************
            if (CH1 > 1600) 
              //if ((GO_RIGHT > 1000)) 
            {
              delay(10);
              while (CH1 > 1600) 
              { 
                //desni ugasi
                digitalWrite(M1_Q2,LOW);
                CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
                CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
              }
            }

            //END **************************  RIGHT  *******************
            digitalWrite(M1_Q2,HIGH);//desni ponovo upali

            CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
            CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
            CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1

          } //end while !rw

        }   //end AUTO_FW          

        previous_milis = millis();     

        CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
        CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
        CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1
      }
    }
    //END**************************************************** FW ************************************  

    //**************************************************** RW ***************************************
    if  (CH2 > 1600) //nazad
    {
      delay(10); 
     if (auto_fw_entered == 1) {delay(500);auto_fw_entered = 0; } //ako je upravo izasao iz AUTO_FW odmori malo
      while (CH2 > 1600)
      {                
        digitalWrite(FAN,HIGH);

        digitalWrite(M1_Q2,LOW); 
        digitalWrite(M2_Q2,LOW);   
        digitalWrite(M1_Q1,LOW); 
        digitalWrite(M2_Q1,LOW); 
        
        digitalWrite(M2_Q3,HIGH); 
        digitalWrite(M1_Q3,HIGH); 
        analogWrite(M2_Q4,30);
        analogWrite(M1_Q4,35);

        previous_milis = millis();  

        CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
        CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
        CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1
        
      }   //end while rw

    } 
    //END**************************************************** RW ***************************************  


    //**************************************************** RIGHT***************************************
    if ( CH1 > 1600) 
    {
      delay(10);  
      while (CH1 > 1600)
      {  
        digitalWrite(FAN,HIGH);
        //levi motor napred,desni polako ili nazad
        //    digitalWrite(M1_Q2,HIGH); //M1  
        //    analogWrite(M1_Q1,150);
        //    digitalWrite(M1_Q1,HIGH); //M1

        digitalWrite(M2_Q3,LOW);    
        digitalWrite(M2_Q4,LOW); 
        digitalWrite(M1_Q1,LOW);
        digitalWrite(M1_Q2,LOW);

        digitalWrite(M2_Q2,HIGH); //M2 FW 
        digitalWrite(M1_Q3,HIGH); //M1 RW
        
        analogWrite(M2_Q1,40); //M2 FW
        analogWrite(M1_Q4,30); //M1 RW SLOW

        //inputs
        CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
        CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
        CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1

        previous_milis = millis();  
      }   

    }//END **************************************************** RIGHT ***************************************


    //**************************************************** LEFT ***************************************
    if (CH1 < 1300) 
    {
      delay(10);  
      while (CH1 < 1300)
      { 
        //desni motor napred,levi polako ili nazad
        digitalWrite(FAN,HIGH);
        //levi motor napred,desni polako ili nazad
        //    digitalWrite(M1_Q2,HIGH); //M1  
        //    analogWrite(M1_Q1,150);
        //    digitalWrite(M1_Q1,HIGH); //M1

        digitalWrite(M1_Q3,LOW); 
        digitalWrite(M1_Q4,LOW); 
        digitalWrite(M2_Q1,LOW);
        digitalWrite(M2_Q2,LOW);

        digitalWrite(M2_Q3,HIGH); //M2 RW
        digitalWrite(M1_Q2,HIGH); //M1 FW
        
        analogWrite(M2_Q4,30);  //M2 RW
        analogWrite(M1_Q1,30); //M1 FW
 
        CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
        CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
        CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1

        previous_milis = millis();  
      } //end while left

    }//END **************************************************** LEFT ***************************************

    digitalWrite(M1_Q2,LOW);
    digitalWrite(M1_Q3,LOW);
    digitalWrite(M2_Q2,LOW);
    digitalWrite(M2_Q3,LOW);

    digitalWrite(M1_Q1,LOW);
    digitalWrite(M1_Q4,LOW);
    digitalWrite(M2_Q1,LOW);
    digitalWrite(M2_Q4,LOW);

    if(millis() - previous_milis > FAN_timeout) {
      digitalWrite(FAN,LOW);
    }
    
    auto_fw_once = 1;
     
    CH1 = (pulseIn (channel_1, HIGH)); // Checks the value of channel1
    CH2 = (pulseIn (channel_2, HIGH)); // Checks the value of channel1
    CH3 = (pulseIn (channel_3, HIGH)); // Checks the value of channel1 

  }// end while(1)

  lcd.clear();
  menu=1;
  return;
} //  ******************************************** end MANUAL ********************************************



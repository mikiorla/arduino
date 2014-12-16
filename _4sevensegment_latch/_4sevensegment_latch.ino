int number=1000, i, x=1;

void setup(){
  pinMode(2, OUTPUT);//Latch
  pinMode(3, OUTPUT);//Clock
  pinMode(4, OUTPUT);//Data
 digitalWrite(2, LOW);
 Serial.begin(115200);

 
}

//115200
void loop(){
sevenseg(1);
  /*

  sevenseg(number);
 
 number= number + x;

if (number>9999)
x= -1;

if (number<0)
x= 1;
*/

}


void sevenseg(int input_number){
short ones, tens, hundreds, thousands;

thousands = int(input_number/1000);
thousands = (thousands<<4)+1;

hundreds = int(input_number/100);
hundreds = hundreds-(int(hundreds/10)*10);
hundreds = (hundreds<<4)+2;

tens = int(input_number/10);
tens = tens-(int(tens/10)*10);
tens = (tens<<4)+4;

ones = input_number-(int(input_number/10)*10);
ones = (ones<<4)+8;

Serial.print(thousands);
Serial.print("   ");
Serial.print(hundreds);
Serial.print("   ");
Serial.print(tens);
Serial.print("   ");
Serial.print(ones);
Serial.println("   ");

//for(i=0; i<5; i++){
if(input_number>=1000){
digitalWrite(2, LOW);
shiftOut(4, 3,MSBFIRST, thousands);//(datapin, clockpin, data)
digitalWrite(2, HIGH);
delay(5);}

if(input_number>=100){
digitalWrite(2, LOW);
shiftOut(4, 3,MSBFIRST, hundreds);//(datapin, clockpin, data)
digitalWrite(2, HIGH);
delay(5);}

if(input_number>=10){
digitalWrite(2, LOW);
shiftOut(4, 3,MSBFIRST, tens);//(datapin, clockpin, data)
digitalWrite(2, HIGH);
delay(5);}

digitalWrite(2, LOW);
shiftOut(4, 3,MSBFIRST, ones);//(datapin, clockpin, data)
digitalWrite(2, HIGH);
delay(5);

digitalWrite(2, LOW);
}//sevenseg

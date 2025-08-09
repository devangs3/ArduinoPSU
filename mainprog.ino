//Code by Devang Sankhala 
//devangs3@gmail.com
//G H Patel College of Engineering & Technology, Gujarat, INDIA

#include <LiquidCrystal.h>

LiquidCrystal lcd(0, 1, 4, 5, 6, 7);//lcd initialization variable
volatile unsigned int count=0;//count=counter for shutdown
unsigned int setpoint=0;  
volatile unsigned int time=count*10;//time for shutdown in minutes 
volatile unsigned long int time_s=(unsigned long int)time*60000;//time for shutdown in milliseconds
volatile unsigned long int now=0;//instant of pressing shutdown button

int current=0, current_temp=1, voltage=0, voltage_temp=1;

void setup() {  
  noInterrupts();// no ISR to be served
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);//relay active
  pinMode(8,OUTPUT);  
  lcd.begin(16,2);//lcd size
  lcd.noCursor();//no cursor to be displayed  
  digitalWrite(8,HIGH);//start beep
  delay(1000);//1s
  digitalWrite(8,LOW);//beep off
  lcd.clear();
  lcd.print("From Maitrey :)");//start msg
  delay(5000);//5s
  lcd.clear();
  attachInterrupt(0,autoshutdown_disp,RISING);//ISR for autoshutdown on pin 2 of uC
  attachInterrupt(1,shutdown,RISING);//ISR for manual shutdown on pin 3 of uC
}

void loop() {  
  int x = analogRead(A2);
//  int y = analogRead(A5);
//  delay(10);
//  if(y>=140)//temp limit from 0(-55celcius) to 307(150celcius), set for LM35 as per requirement, here it is 68celcius
//    {
//      temp_shutdown();
//    }
  setpoint= map(x, 0, 1023, 0, 255);// input of pot
  delay(10);
  current_temp = analogRead(A0);//current sense
  delay(10);
  voltage_temp = analogRead(A1);//voltage sense
  delay(10);
  interrupts();
  if(current_temp != current || voltage_temp != voltage){
    voltage=voltage_temp;
    current=current_temp;
    measure_disp();      
  }
  
  if(count != 0){
   if(millis()>now && (millis()-now)>=time_s){
    shutdown();
   }  
  }     
  analogWrite(10,255-setpoint);
 delay(100);  
}

void shutdown()//system shutdown ISR
{
  digitalWrite(8,HIGH);//stop beep
  delay(1000);//1s
  digitalWrite(8,LOW);//beep off
  lcd.clear();
  lcd.print("BYEEE :)");
  delay(1000);
  digitalWrite(9,LOW);
}

void temp_shutdown()//system shutdown ISR
{
  unsigned int i=3;
  lcd.clear();
  lcd.print("TEMP OVERLOAD X(");
  for(i=0;i<=3;i++)//temperature alarm
    {
      digitalWrite(8,HIGH);
      delay(800);//1s
      digitalWrite(8,LOW);  
    }
  delay(1000);
  digitalWrite(9,LOW);
}

void measure_disp()//display voltage and current
{
  lcd.clear();
  lcd.print("VOLTAGE: ");
  lcd.print(float(voltage+1)*0.0585);
  lcd.print("V");
  lcd.setCursor(0,1);
  lcd.print("CURRENT: ");
  lcd.print(float(current+1)*0.009);
  lcd.print("A");
}

void autoshutdown_disp()//ISR, shutdown selection
{
  now=millis();
  lcd.clear();
  lcd.print("SHUTDOWN MINS");
  lcd.setCursor(0,1);
  count++;
  if(count>5 || count<0){
    count=0;
  }
  
  time=count*10;//time for shutdown in minutes 
  time_s=(unsigned long int)time*60000;//time for shutdown in milliseconds
  
  switch(count){
  default:
    lcd.print("TIMER OFF");
    break;
    ;
  case 1:
    lcd.print(time);//10 mins    
    break;
    ;
  case 2:
    lcd.print(time);//20 mins
    break;
    ;
  case 3:
    lcd.print(time);//30 mins
    break;
    ;
  case 4:
    time=time*6/4;
    lcd.print(time);//1 hr
    break;
    ;  
  case 5:
    time=time*12/5;
    lcd.print(time);//2 hrs
    break;
    ;
  }
  delay(1000);  
}


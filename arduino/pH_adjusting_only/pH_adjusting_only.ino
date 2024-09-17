#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Servo.h>
#include <TM1637Display.h>


#define CLK 5
#define DIO 4
TM1637Display display(CLK, DIO);
int seg7_ph=438;

int myph=431;



Servo myservo;
int c_fact;

virtuabotixRTC myRTC(3, 2, 1);//clk,dat,rst
float time_nw,ph_nw;

float calibration_value = 33.64;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

int pos = 0;

const int p_motor=8;//pond
const int d_motor=9;//distilled
const int a_motor=10;//acid
const int b_motor=11;//base
const int out_motor=12;//drain

const int servo_pin=6;//servo pin
// connect ph pin to A0

const int tp=2000; //filling water from pond
const int tb=2000; //spraying base
const int ta=2000; //spraying acid
const int tout=2000; //empty container
const int td=2000; //filling distilled water


void food_dispense()
{
  for(int i=0;i<3;i++)
  {
    for (pos = 0; pos <= 180; pos += 1) { 
    myservo.write(pos);            
    delay(15);                
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);             
    delay(15);                      
  }
  }
}

float ph_read()
{
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 
 return ph_act;
}

void ph_set()
{ 
  digitalWrite(out_motor, HIGH);
  delay(2000);
  digitalWrite(out_motor, LOW);
  delay(2000);
  digitalWrite(p_motor, HIGH);
  delay(tp);
  digitalWrite(p_motor, LOW);
  delay(2000);
  float present_ph=10.00;
  if(present_ph<6.5)
  { 
    c_fact= tb + ((6.5-present_ph)*1000) ;
    digitalWrite(b_motor, HIGH);
    delay(2000);
    digitalWrite(b_motor, LOW);
  }
  else if(present_ph>7.5)
  {
    digitalWrite(a_motor, HIGH);
     c_fact= tb + ((present_ph-7.5)*1000) ;
    delay(2000);
    digitalWrite(a_motor, LOW);
  }

  display.clear();
  seg7_ph=present_ph*100;
  Serial.println(present_ph);
  
  display.showNumberDec(myph,true);

  digitalWrite(out_motor, HIGH);
  delay(tout);
  digitalWrite(out_motor, LOW);
  delay(1000);
  digitalWrite(d_motor, HIGH);
  delay(td);
  digitalWrite(d_motor, LOW);
  delay(1000);
  digitalWrite(out_motor, HIGH);
  delay(35000);
  digitalWrite(out_motor, LOW);
  delay(1000);
  digitalWrite(d_motor, HIGH);
  delay(td);
  digitalWrite(d_motor, LOW);
  delay(5000);
}



void setup()
{
  Serial.begin(9600);
  myRTC.setDS1302Time(30, 19,23, 3, 27, 3, 2024);

  display.setBrightness(5);



  pinMode(p_motor, OUTPUT); 
  pinMode(d_motor, OUTPUT); 
  pinMode(a_motor, OUTPUT); 
  pinMode(b_motor, OUTPUT); 
  pinMode(out_motor, OUTPUT); 



}

void loop() 
{
  myRTC.updateTime();
  time_nw=myRTC.hours*100+myRTC.minutes+myRTC.seconds*.01;


  if(analogRead(A5)==0)
  {ph_set();
   food_dispense();
  }
   
  // else if(time_nw==2320.00)
  // {
  //   ph_set();
  //   food_dispense();
  // }
  // else if(time_nw==900.10)
  // {
  //   food_dispense();
  // }
  // else if(time_nw==1200.20)
  // {
  //   ph_set();
  // }
  // else if(time_nw==1500.20)
  // {
  //   ph_set();
  // }
  // else if(time_nw==1800.20)
  // {
  //   ph_set();
  // }
  // else if(time_nw==2100.20)
  // {
  //   food_dispense();
  // }
  // else if(time_nw==0.20)
  // {
  //   ph_set();
  // }
   ph_set();
  delay(1000);
}
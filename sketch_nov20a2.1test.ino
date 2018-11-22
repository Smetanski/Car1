
#include <Servo.h>
Servo servo1;
int pos=90;
int right0;
int rightwor;
int left180;
int forward90;
int cm,x,y,t;   
int m[10],s=0,mean,suma=0;
int measure[12];
int range[5];
int i=0;
int IN_1 = 7;
int IN_2 = 6;
int ENA = 2;
int ENB = 3;
int IN_3 = 5;
int IN_4 = 4;
int echoPin = 9; 
int trigPin = 8; 
 //int rotation = 0;
 #define r 700
  #define S 600
void setup()
{
  
Serial.begin (9600);
pinMode(IN_1,OUTPUT);
pinMode(IN_2,OUTPUT);
pinMode(ENA,OUTPUT);
pinMode(IN_3,OUTPUT);
pinMode(IN_4,OUTPUT);
pinMode(ENB,OUTPUT);
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
servo1.attach(10);
   
}
void motorAforward( ) {
 digitalWrite (IN_1, LOW);
 digitalWrite (IN_2, HIGH);
 analogWrite(ENA,0);
}
  void motorBforward( ) {
 digitalWrite (IN_3,LOW );
 digitalWrite (IN_4, HIGH);
 analogWrite(ENB,0);
}
void motorAbackward( ) {
 digitalWrite (IN_1, HIGH);
 digitalWrite (IN_2, LOW);
 analogWrite(ENA,120);
}
void motorBbackward( ) {
 digitalWrite (IN_3, HIGH);
 digitalWrite (IN_4, LOW);
 analogWrite(ENB,120);
}
void motorAstop( ) {
 digitalWrite (IN_1, HIGH);
 digitalWrite (IN_2, HIGH);
 analogWrite(ENA,0);
}
void motorBstop( ) {
 digitalWrite (IN_3, HIGH);
 digitalWrite (IN_4, HIGH);
 analogWrite(ENB,0);
}
void forward ( ) {
 motorAforward();
 motorBforward();
}
void backward ( ) {
 motorAbackward();
 motorBbackward();
}
void right ( ) {
 motorAforward();
 motorBbackward();
}
void left () {
 motorBforward();
 motorAbackward();
}
void breakMotors() {
 motorAstop();
 motorBstop();
}
int Duration()
{
  int duration, cm; 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58;
 // Serial.print(cm); 
//  Serial.println(" ");  
 // delay(100);
 return cm;
}


void loop() {
  pos=90;
  forward();
 servo1.write(pos);
 x=Duration();
 Serial.println(x);  
 while(x>40){
  x=Duration();
  forward ();
  delay(250);
  motorAstop();
  motorBstop();
  delay(100);
  if(x<40){
    break;
    }
 }
  motorAstop();
  motorBstop();
  delay(1000);
  for (pos = 0; pos <= 12; pos ++) 
 { 
   servo1.write(pos*15);
   measure[pos]=Duration();
   delay(100);
 }
  right();
  delay(400);
  breakMotors();
  delay(1500);
  forward ();
  delay(r);
  breakMotors();
  delay(1000);
  left ();
  delay(320);
  breakMotors();
  delay(2000);
  forward ();
  delay(r);
  breakMotors();
  delay(1000);
  left ();
  delay(320);
  breakMotors();
  delay(2000);
  forward ();
  delay(r);
  breakMotors();
  delay(2000);
  servo1.write(180);
  delay(300);
  right();
  delay(350);
  forward ();
  delay(r);
   breakMotors();
  delay(20);
}

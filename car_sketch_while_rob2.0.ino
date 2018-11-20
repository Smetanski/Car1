
#include <Servo.h>
Servo servo1;
int pos=90;
int right0;
int rightwor;
int left180;
int forward90;
int cm,x,y;   
int m[10],s=0,mean,suma=0;
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
 motorBforward();
 motorAbackward();
}
void left () {
 motorAforward();
 motorBbackward();
}
void breakMotors() {
 motorAstop();
 motorBstop();
}
int Duration()
{
  int duration, cm; 
 // digitalWrite(trigPin, LOW); 
 // delayMicroseconds(10); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58;
 // Serial.print(cm); 
//  Serial.println(" ");  
  //delay(100);
 return cm;
}
void loop()
{
 forward ();
  delay(150);
  motorAstop();
  motorBstop();
  delay(100);
  //while(1){
  pos = 90;
  servo1.write(pos);
  delay(10);
  x = Duration();
  delay(100);
   Serial.println(x);
 while(x < 30 && pos==90)
 { 
 
  motorAstop();
  motorBstop();
  delay(1000);
 for (pos = 0; pos <= 180; pos +=90) 
 { 
   servo1.write(pos);
  // x = Duration();
   delay(250);
   if(pos==0){
     x = Duration();
     right0=x;
    }
    if(pos==90){
     x = Duration();
    forward90=x;
   }
   if(pos==180){
     x = Duration();
    left180=x;
   } delay(2000);
  }
   Serial.println(" "); 
  Serial.println(right0);
  Serial.println(forward90);
  Serial.println(left180); 
    Serial.println(" "); 
 /* if(forward90<30)
  {
    break;
  }*/
 if(right0>left180 )
    {
      right();
        
      delay(150); /*
      rightwor=Duration();
      delay(250);
      if(rightwor>30){
        forward();
        }*/
    }
 if (right0<left180)
 { 
  left();
   delay(150); /*
  rightwor=Duration();
      delay(250);
      if(rightwor>30){
        forward();
        }   */                                                     
     }
 }
 
 
 
  //}
}




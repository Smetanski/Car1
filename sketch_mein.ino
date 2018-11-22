 //серво
#include <Servo.h>
#include <math.h>
Servo servoa;
int pos=90;
double D;
 int cm , x;   
int k = 0;
#define N 5
//мотор А
int IN_1 = 7;
int IN_2 = 6;
int ENA = 2;
//мотор В
int ENB = 3;
int IN_3 = 5;
int IN_4 = 4;
//відстань
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
servoa.attach(10);
   
}
//функції окремо до моторів
void motorAforward( ) {
 digitalWrite (IN_1, LOW);
 digitalWrite (IN_2, HIGH);
 analogWrite(ENA,50);
}
  void motorBforward( ) {
 digitalWrite (IN_3,LOW );
 digitalWrite (IN_4, HIGH);
 analogWrite(ENB,50);
}
void motorAbackward( ) {
 digitalWrite (IN_1, HIGH);
 digitalWrite (IN_2, LOW);
 analogWrite(ENA,50);
}
void motorBbackward( ) {
 digitalWrite (IN_3, HIGH);
 digitalWrite (IN_4, LOW);
 analogWrite(ENB,50);
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
// Функції вперед,назад і тд
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
  duration = pulseIn(echoPin, HIGH ,8000); 
  cm = (duration*.0343)/2;
//  Serial.print(cm); 
 // Serial.println(" "); 
   
 return cm;
}
void loop()
{
 breakMotors();
 delay(200);
int m[N],s=0,mean,suma=0;
int D = 0;
int S = 0;
int d[10];

Serial.println("Start"); 
for(int i  = 0 ;i <N ; i++)
  {
 
  m[i] = Duration();
  s=s+m[i];
   }

 /*for(int i  = 0 ;i <N ; i++)
  {
  Serial.print(m[i]); 
  Serial.print(" ");
 
 
   }
   Serial.println("End");
   Serial.print(k); 
  k++;*/
 
   mean = s/5;
for(int i  = 0 ;i <N ; i++)
   {
   if(m[i] == 0){
   forward();
   delay(100);
   breakMotors();
   delay(200);
   }
   else{
    if(mean>40){
      forward();
   delay(100);
   breakMotors();
   delay(200);
    }
    else{
      breakMotors();
      delay(500);
    }
   }
   }
/*   Serial.println(" ");
  Serial.println("Mean");
   
  mean = s/10;
  Serial.println(mean); 
  for(int j = 0; j <= 10 ; j++)
    {
     for(int i  = 0 ;i <=10 ; i++)
  {
  d[j] = m[i] - mean ;
  S =S +( d[j]*d[j]); 
  
  }
    }
   Serial.println("Desp"); 
    D = S/10 ; 
    Serial.println(D); 
    delay(5000);*/
}

 


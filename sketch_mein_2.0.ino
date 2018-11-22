 //серво
#include <Servo.h>
#include <math.h>
Servo servoa;
int pos=90;

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
int  leftMsr()
{ 
  int leftAvg = 0 ;
  servoa.write(180);
  delay(100);
 leftAvg = Measure();
if (leftAvg <30 )
{
  return 1 ; 
}
else
{
  if ( leftAvg > 30 || leftAvg == 0 )
  {
    return 0 ; 
  }
}

}
int  forwardMsr()
{ 
  int forwardAvg = 0 ;
  servoa.write(90);
  delay(100);
 forwardAvg = Measure();
if (forwardAvg <30 )
{
  return 1 ; 
}
else
{
  if ( forwardAvg > 30 || forwardAvg == 0 )
  {
    return 0 ; 
  }
}

}
int Measure()
{
   int avg = 0 , Sum = 0 ;
  int K[N] ; 
    for (int i = 0 ; i < N ; i++)
  {
    K[i] = Duration();
    Sum  = Sum + K[i];
  }
   avg = Sum / 5 ; 
   for(int i  = 0 ;i <N ; i++)
   {
   if(K[i] == 0)
   {
    avg = 0;
   }

   }
   return avg ; 
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
  int f1 = 0 ; 
  int f2 = 0 ; 
 breakMotors();
 delay(200);
  f1 = forwardMsr();


      if(f1==0){
        forward();
        delay(100);
        breakMotors();
        delay(200);
        f2=leftMsr();
           if(f2==1){
               forward();
               delay(100);
               breakMotors();
               delay(200);         
            }
            else
            { 
              if(f1==0&&f2==0){
                 forward();
                 delay(100);
                 breakMotors();
                 delay(200); 
                 right();
                 delay(200);}
            }
      }
      else{
        left();
        delay(100);
        forward();
        delay(100);
        breakMotors();
        delay(200);
      }
   

}

 

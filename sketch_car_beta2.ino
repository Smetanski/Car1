 // Підключаємо серво
#include <Servo.h>
#include <math.h>
Servo servoa;
// оголошуємо основні змінні
int pos=90;
int drive = 0 ;
#define N 10
//Піни підєднання до  мотор А
int IN_1 = 7;
int IN_2 = 6;
int ENA = 3;
//Піни підєднання до  мотор В
int ENB = 11;
int IN_3 = 5;
int IN_4 = 4;
// Підключення ультразвукового сенсора
int echoPin = 9; 
int trigPin = 8; 
void setup()
{
  // управління драйвером
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
//Основні функції управлення моторами
void motorAforward(int n ) {  //мотор А вперед 
 digitalWrite (IN_1, LOW);
 digitalWrite (IN_2, HIGH);
 analogWrite(ENA,n);
}
  void motorBforward(int n ) { // мотор В вперед 
 digitalWrite (IN_3,LOW );
 digitalWrite (IN_4, HIGH);
 analogWrite(ENB,n);
}
void motorAbackward(int n ) { // мотор А назад 
 digitalWrite (IN_1, HIGH);
 digitalWrite (IN_2, LOW);
 analogWrite(ENA,n);
}
void motorBbackward(int n ) { // мотор В назад
 digitalWrite (IN_3, HIGH);
 digitalWrite (IN_4, LOW);
 analogWrite(ENB,n);
}
void motorAstop( ) { // мотор А стоп
 digitalWrite (IN_1, HIGH);
 digitalWrite (IN_2, HIGH);
 analogWrite(ENA,0);
}
void motorBstop( ) {  //мотор В стоп
 digitalWrite (IN_3, HIGH);
 digitalWrite (IN_4, HIGH);
 analogWrite(ENB,0);
}
// Основні функції руху вперед, назад, вліво, вправо, стоп.
void forward ( int n ) {
 motorAforward(n+18);
 motorBforward(n);
}
void backward ( ) {
 motorAbackward(70);
 motorBbackward(70);
}
void left(int n ) {
 motorAforward(n);
 motorBbackward(n);
}
void right (int n) {
 motorAbackward(n);
 motorBforward(n);
 
}
void breakMotors() {
 motorAstop();
 motorBstop();
}
void steep()  // функція руху покроково
{
    forward(70);
    delay(200);
    breakMotors();
    delay(100);
}
int Duration()  // Основна функція вимірювання
{
  int duration=0;
  int cm=0; 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH ,8000); 
  cm = (duration*.0343)/2;
   if(cm==0)
      {
        cm=100;
      }
   return cm;
}
int Measure2()    // Дві наступні функція реалізують ідею вимірювання, яка полягає в тому, що ми 5 значень вимірюємо і шукаємо середнє, яке в вважаємо як значення відстанні.
{
    int avg = 0 , Sum = 0 ;
    int M[N] ; 
    for (int i = 0 ; i < N ; i++)
      {
        M[i] = Duration();
        Sum  = Sum + M[i];
      }
   avg = Sum / 10 ; 
   return avg;
   }
int Measure1()
{
    int avg = 0 , Sum = 0 ;
    int K[N] ; 
    for (int i = 0 ; i < N ; i++)
      {
          K[i] = Duration();
          Sum  = Sum + K[i];
      }
   avg = Sum / 10 ; 
   for(int i  = 0 ;i <N ; i++)
    {
      if(K[i] == 0)
        {
          avg = 0;
        }
    }
    return avg ; 
}                            // Наступні функції реалізують систему довороту машини, якщо оминаюча перешкода буде не під прямим кутом до машинки.
                             // тобто, якщо значення вийде за дозволенні межі, машинка виконає доворот.
                                  
                                          //Функція вибору напрямку повороту при перешкоді. Спочатку записується массив даних, що відповідаю напрямку перед машинкою від 0 до 180'
                                     // Далі шукається максимальне значення масиву, відповідно до нього машинка повертає на відповідниий кут і відповідну сторону.
int correctside()
{   
  int a[12];
  int A = 0 ;
  for( int n=0;n<12;n++)
  {
   servoa.write(n*15);
   
   A = Measure2();               // <===== заповнення масиву значеннями вимір.
   if(A==0)
    {
      A=100;  
    }
   a[n]= A ;
   delay(200);
  }
  if((a[1] == 0 || a[1] >60) && (a[10] == 0 || a[10] >60))
  {
   right(125);
   delay(190);
   return 1;
  }
  int max1 = a[0];
  int index = 0;
  for (int z = 0; z < 12; z++)
  {
    if (a[z] > max1)            // <===== Пошук макс. значення
   {
    max1 = a[z];
    index = z;
    index++;
   }
  }
   
  if(index < 7)                // <===== 
  {

  right(125);
  delay(190); 
  return 1 ;
 }
   else
 {
 
    left(125);
    delay(190);
    return 2;  
 }                                       // Завдяки наступним 3 функціям, відбувається орієнтація машинки під час функції обїзду перешкоди .
}                                         // Якщо перед нею є щось, то функція повертає 1, ні - 0.
int  leftMsr()
{ 
  int leftAvg = 0 ;
  servoa.write(180);
  leftAvg = Measure2();
  delay(300);
if (leftAvg <15 && leftAvg != 0  )
  {
    return 1 ; 
  }
else
{
    if ( leftAvg > 15 || leftAvg == 0 )
  {
    return 0 ; 
  }
}
}
int rightMsr()
{ 
  int rightAvg = 0 ;
  servoa.write(0);
  rightAvg = Measure2();
  delay(300);
if (rightAvg <15 && rightAvg != 0  )
  {
    return 1 ; 
  }
else
{
    if ( rightAvg > 15 || rightAvg == 0 )
  {
    return 0 ; 
  }
}

}
int  forwardMsr()
{ 
  int forwardAvg = 0 ;
  servoa.write(90);
  forwardAvg = Measure1();
if (forwardAvg <20 && forwardAvg != 0  )
  {
    return 1 ; 
  }
else
  {
    if ( forwardAvg > 20 || forwardAvg == 0 )
    {
      return 0 ; 
    }
  }
}
 
void loop()
{
  int p = 0 ; 
  int k = 0 ; 
  int f1 = 0 ; 
 f1 = forwardMsr();
 if (drive ==0)
 {
  while(f1 == 0 )
  {
  
    steep();
    f1 = forwardMsr();
      if(f1 == 1 )
        {
          drive = 1 ;
          break; 
        }
  }
 }else
 {
  while(1)
  {
    if(p<10)
    {
     p++;
     steep(); 
    }else{breakMotors();}
    
    
  }
 }
  steep();
  delay(100);
  steep();
  delay(500);
  steep();
  delay(100);
  k = correctside();
  delay(200);
  breakMotors();
  delay(100);
  while(k == 1)
    { 
      int kurve = 0;
      servoa.write(180);
      delay(400);
      int leftcorrect = 0 ;
      leftcorrect = Measure2();
      servoa.write(90);
      delay(400);
      int forcorrectL =  Measure2();
      delay(300);
        if( forcorrectL <15 && leftcorrect < 24 )
           {
              right(150);
              delay(260);
           }   
        if(leftcorrect >= 20)
           {
           left(200);
           delay(50);
           }              
       if(leftcorrect < 20)
          {
           right(200);
           delay(40);          
          }   
       if(leftcorrect > 50)
          {
              forward(100);
              delay(540);
              breakMotors();
              delay(100);
              left(200);
              delay(185);
              breakMotors();
              delay(100);
              steep();
              delay(100);
              servoa.write(90);
             delay(400);
             int frl =  Measure2();
             if(frl >= 90)
             { 
               break;
             }
              servoa.write(180);
              delay(100);
              kurve = Measure2();
                while(kurve > 28)
                    {
                       servoa.write(180);
                       delay(100);                      
                       left(80);
                       delay(40);
                       kurve = Measure2();
                    }
             
          }
      
          steep(); 
          delay(100);   
      }  
     while(k == 2)
      {
        int kurve1 = 0;
        servoa.write(0);
        delay(400);
        int rightcorrect = 0 ;
        rightcorrect = Measure2();
        servoa.write(90);
        delay(400);
        int forcorrectR = Measure2();
        delay(100);
        if(forcorrectR <15 && rightcorrect <24)
             {
              left(130);
              delay(230);
             } 
            if(rightcorrect >= 15)
              {
                right(200);
                delay(50);               
              }
            if(rightcorrect<15)
              {
                left(220);
                delay(40);
              }
             
             if(rightcorrect > 45)
             {
              forward(100);                                           
              delay(500);
              breakMotors();
              delay(100);
              right(200);
              delay(185);
              breakMotors();
              delay(100); 
              steep();
              delay(100);
              right(70);
              delay(100);
              steep();
              servoa.write(90);
             delay(400);
             int fr2 =  Measure2();
             if(fr2 >= 90)
              { 
               break;
              }       
              servoa.write(0);
              delay(100);
              kurve1 = Measure2();
              while(kurve1 > 25)
                    {
                       servoa.write(0);
                       delay(100);                      
                       right(100);
                       delay(50);
                       kurve1 = Measure2();
                    }  
                    
             }
              steep();
             delay(100);
       
      }
}   

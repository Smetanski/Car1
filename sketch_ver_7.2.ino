 // Підключаємо серво
#include <Servo.h>
#include <math.h>
#include <Wire.h> 
#define address 0x1E 
Servo servoa;
// оголошуємо основні змінні
int pos=90;
int f4,f3;
 int cm , x;   
int f5;
#define N 5
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
Wire.begin();
Wire.beginTransmission(address);
Wire.write(0x02);
Wire.write(0x00);
Wire.endTransmission();
   
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
  int duration, cm; 
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
   avg = Sum / 5 ; 
   for(int i  = 0 ;i <N ; i++)
      {
        if(M[i] == 0)
          {
             avg = 0;  // Виходячи з технічний умов сенсора( радіус дії якого нас( 1 метр) нас задовільняє).
                        //Коли відстань більша 1-1.2 м. то сенсор видає значеня 0, що ми і використовуємо.
          }
      }
   return avg ; 
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
   avg = Sum / 5 ; 
   for(int i  = 0 ;i <N ; i++)
    {
      if(K[i] == 0)
        {
          avg = 0;
        }
    }
    return avg ; 
}                        // Наступні функції реалізують систему довороту машини, якщо оминаюча перешкода буде не під прямим кутом до машинки.
void correctsteepleft()  // тобто, якщо значення вийде за дозволенні межі, машинка виконає доворот.
{
  steep();
  delay(100);

 steep();
 delay(100);
}
void correctsteepright()
{
  steep();
  delay(40);
 
 steep();
 delay(100);
}                                      //Функція вибору напрямку повороту при перешкоді. Спочатку записується массив даних, що відповідаю напрямку перед машинкою від 0 до 180'
                                       // Далі шукається максимальне значення масиву, відповідно до нього машинка повертає на відповідниий кут і відповідну сторону.
int correctside()
{   
  int a[12];
  for( int n=0;n<12;n++)
  {
   servoa.write(n*15);
   int A = 0 ;
   A = Duration();
   if(A==0)
    {
      A=100;  
    }
   a[n]= A ;
   delay(200);

  }
  int max1 = a[0];
  int index = 0;
  for (int z = 0; z < 12; z++)
  {
    if (a[z] > max1)
   {
    max1 = a[z];
    index = z;
    index++;
   }
  }
   
  if(index < 7) 
  {
   int chao=0;
   int po=0;
   int po2=0;
  for( int e=5;e>=0;e++)
  {
    po=a[e-1];
    chao=a[e]-po;
   if(abs(chao) > 50)
    {
      break;
      po2=e+1;
    }
    right(125);
    delay(po2*125); 
   return 1 ;
  }
 }
   else
 {
   int chao1=0;
   int po3=0;
   int po4=0;
  for( int e=6;e<=12;e++)
  {
   po3=a[e+1]; 
   chao1=a[e]-a[e+1];
    if(chao1 > 50)
      {
       break;
       po4=(e+1)-5;
      }
   }
    left(125);
    delay(po4*125);
    return 2;
      
   }
 }                                       // Завдяки наступним 3 функціям, відбувається орієнтація машинки під час функції обїзду перешкоди .
                                         // Якщо перед нею є щось, то функція повертає 1, ні - 0.
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
  delay(200);
if (forwardAvg <25 && forwardAvg != 0  )
  {
    return 1 ; 
  }
else
  {
    if ( forwardAvg > 25 || forwardAvg == 0 )
    {
      return 0 ; 
    }
  }

}

void right_turn()
{
int   f4;
  do{ 
      delay(300);
      f5=leftMsr();
      delay(200);
      f4=forwardMsr();
      delay(200);
      Serial.println("er");
      //correctsteepright();
     steep();
    }
  while(f4==1 &&f5==0); 
  forward(100);
  delay(600);
  breakMotors();
  delay(150);
  left(125);
  delay(300);
  steep();
  delay(200);
  steep(); 
}
void left_turn()
{
  int   f4;
  do{ 
      delay(300);
      f3=rightMsr();
      delay(200);
     f4=forwardMsr();
      delay(200);
      Serial.println("el");
      delay(300);
     //correctsteepleft();
  steep();
    }
  while( f4==1 && f3 == 0);
  forward(100);
  delay(600);
  breakMotors();
  delay(150);
  right(125);
  delay(300);
  steep();
  delay(200);
  steep();
}
void loop()
{
  int k = 0 ; 
  int f1 = 0 ; 
  int f2 = 0 ;
  int f3 = 0;
  int f4 = 0;
  int f5 = 0; 
 f1 = forwardMsr(); 
 Serial.println(f1);
 int x,y,z; //triple axis data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(address, 6);
  if(6<=Wire.available())
    {    
      x = Wire.read()<<8;    x |= Wire.read(); 
      z = Wire.read()<<8;    z |= Wire.read(); 
      y = Wire.read()<<8;    y |= Wire.read(); 
    }
  if 
while(f1 == 0 &&  <=x<= )
  {
    steep();
    Serial.println(" "); 
    f1 = forwardMsr();
    Serial.println(f1); 
    Serial.print("f1"); 
    if(f1 == 1 )
      {
        break;
      }
  }
  steep();
  delay(500);
   steep();
  delay(500);
   steep();
  delay(500);
  k = correctside();
  Serial.println("сторона = ");
  Serial.print(k);
  delay(300);
  breakMotors();
  delay(100);
 
  if (k==1)
  {
    do{ 
  
      delay(300);
      f5=leftMsr();
      delay(200);
      f4=forwardMsr();
      delay(200);
      Serial.println("er");
  servoa.write(180);
  delay(400);
  int leftcorrect = 0 ;
  leftcorrect = Duration();
 delay(300);
  if(leftcorrect > 23 && leftcorrect <30)
    {
      Serial.println("work>25"); 
      left(250);
      delay(80);
    }
  if(leftcorrect< 14)
    {
      Serial.println("work<15"); 
      right(250);
      delay(80);
    }
      steep();
    }
  while(f4==1 && f5==0 && k == 1); 
  }
  do{ 
      delay(300);
      f3=rightMsr();
      delay(200);
     f4=forwardMsr();
      delay(200);
      Serial.println("el");
      delay(300);
  servoa.write(0);
  delay(400);
  int rightcorrect = 0 ;
  rightcorrect = Duration();
 delay(100);
  if(rightcorrect > 23 && rightcorrect <30)
    {
      Serial.println("work>23"); 
      left(250);
      delay(80);
    }
  if(rightcorrect<14)
    {
      Serial.println("work<14"); 
      right(250);
      delay(80);
    }
      steep();
    }
  while( f4==1 && f3 == 0 && k == 2);
  forward(100);
  delay(600);
  breakMotors();
  delay(150);
  if(k==1)
    {
      left(125);
      delay(300);
    }
  else 
    {
      right(125);
      delay(300);
    }
  steep();
  delay(200);
  steep();
}

 

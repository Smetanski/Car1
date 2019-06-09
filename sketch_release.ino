// Підключаємо серво
#include <Servo.h>
#include <math.h>
Servo servoa;
// оголошуємо основні змінні
int pos = 90;                      // позиція серво за замовчуванням.
int get_Straight = 0 ;              // змінна для контролю відстані для вирівнювання.
int drive = 0 ;                    // змінна для визначення початку і кінця роботи алгоритму.
#define N 10                       // місткість масиву із виміряними даними.
//Піни підєднання до  мотор А
int IN_1 = 7;                   // IN-піни для подачі логічного рівня 0 або 1.
int IN_2 = 6;                   //  ENA / ENB - піни для контролю потужності моторів.
int ENA = 3;
//Піни підєднання до  мотор В
int ENB = 11;
int IN_3 = 5;
int IN_4 = 4;
// Підключення ультразвукового сенсора
int echoPin = 9;                    // приймаючий пін дальнометра.
int trigPin = 8;                    // пін подачі вихідного сигналу.
void setup()
{
  // управління драйвером.  Присвоєння пінам властивостей прийому/предачі даних.
  Serial.begin (9600);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoa.attach(10);

}
//Основні функції управлення моторами
void motorAforward(int n ) //мотор А вперед
{
  digitalWrite (IN_1, LOW);
  digitalWrite (IN_2, HIGH);
  analogWrite(ENA, n);
}
void motorBforward(int n ) // мотор В вперед
{
  digitalWrite (IN_3, LOW );
  digitalWrite (IN_4, HIGH);
  analogWrite(ENB, n);
}
void motorAbackward(int n ) // мотор А назад
{
  digitalWrite (IN_1, HIGH);
  digitalWrite (IN_2, LOW);
  analogWrite(ENA, n);
}
void motorBbackward(int n )// мотор В назад
{
  digitalWrite (IN_3, HIGH);
  digitalWrite (IN_4, LOW);
  analogWrite(ENB, n);
}
void motorAstop( ) // мотор А стоп
{
  digitalWrite (IN_1, HIGH);
  digitalWrite (IN_2, HIGH);
  analogWrite(ENA, 0);
}
void motorBstop( )//мотор В стоп
{
  digitalWrite (IN_3, HIGH);
  digitalWrite (IN_4, HIGH);
  analogWrite(ENB, 0);
}
// Основні функції руху вперед, назад, вліво, вправо, стоп.
void forward ( int n )
{
  motorAforward(n + 18);
  motorBforward(n);
}
void backward (int n  )
{
  motorAbackward(n);
  motorBbackward(n);
}
void left(int n )
{
  motorAforward(n);
  motorBbackward(n);
}
void right (int n)
{
  motorAbackward(n);
  motorBforward(n);
}
void breakMotors()
{
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
  int duration = 0;
  int cm = 0;
  digitalWrite(trigPin, LOW);                     //подання напруги для ультразвукового імпульсу.
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 8000);        // прийняття відбитого імпульсу, встановлено обмеження в 100 сантиметрів , при перевищенні отримуємо 0.
  cm = (duration * .0343) / 2;                    // перетворення відбитого імпульсу в відстань у сантиметрах.
  if (cm == 0)                                   // якщо отримана відстань більша за допустиму, то повертаємо 100 см.
  {
    cm = 100;
  }
  return cm;
}
// Дві наступні функція реалізують алгоритм вимірювання, з 10 виміряних значень визначаємо серднє і приймаємо його як відстань.
int Measure2()
{
  int avg = 0, Sum = 0 ;
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
  int avg = 0, Sum = 0 ;
  int K[N] ;
  for (int i = 0 ; i < N ; i++)
  {
    K[i] = Duration();
    Sum  = Sum + K[i];
  }
  avg = Sum / 10 ;
  for (int i  = 0 ; i < N ; i++)
  {
    if (K[i] == 0)
    {
      avg = 100;
    }
  }
  return avg ;
}
//Спочатку записується массив даних, що відповідаю напрямку перед машинкою від 0 до 180.
// Далі шукається максимальне значення масиву, відповідно до нього машинка повертає на відповідниий кут і відповідну сторону.
int correctside()  //Функція вибору напрямку обїзду при перешкоді.
{
  int distant_arr[12];
  int Msr_result = 0 ;
  for ( int n = 0; n < 12; n++)
  {
    servoa.write(n * 15);                   //зміна позиції сервомотора і датчика на 15 градусів після кожного вимірювання.

    Msr_result = Measure2();               //  заповнення масиву значеннями вимірування.
    if (Msr_result == 0)
    {
      Msr_result = 100;
    }
    distant_arr[n] = Msr_result ;
    delay(200);
  }
  if ((distant_arr[1] == 0 || distant_arr[1] > 60) && (distant_arr[10] == 0 || distant_arr[10] > 60)) // поворот на право за замовчуванням
  {
    right(125);
    delay(190);
    return 1;
  }
  int max1 = distant_arr[0];
  int index = 0;
  for (int z = 0; z < 12; z++)
  {
    if (distant_arr[z] > max1)            // <===== Визначення індексу сегмента з найбільшою відстанню.
    {
      max1 = distant_arr[z];
      index = z;
      index++;
    }
  }

  if (index < 7)               // <===== Якщо індекс менший 7 - повертаємо направо і повертаємо 1  , якщо ні - наліво і повертаємо 2.
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
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------Алгоритм роботи платформи---------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  //ініціалізація змінних
  int steep_counter = 0 ;           //кількість кроків після для завершення обїзду перешкоди.
  int side_choose = 0 ;             //змінна-тригер для вибору сторони обїзду перешкоди.
  int front_Msr = 0 ;               //змінна для запису результату вимірювання перд собою.
  get_Straight = 0;                 //обнулення змінної, що контролює відстань під час вирівнювання платформи
  servoa.write(90);
  delay(200);
  front_Msr = Measure1();
  delay(1000);

  //перевірка умови виходу з режиму обізду
  if (drive == 0)                      // 0 - перешкоду попередньо не обїждали
  {
    while (front_Msr > 20 )             // відстань попереду більша 20см 1 крок вперед і повторне вимірювання
    {

      steep();
      front_Msr = Measure1();
      if (front_Msr <= 20 )           //відстань менша 20см перехід до режиму обїзду перешкоди
      {
        drive = 1;
        break;
      }
    }
  } else if ( drive == 1)               // 1 - здійснено попередній обїзд перешкоди , вихід з режиму обїзду закінчення руху після 10 кроків вперед
  {
    while (1)
    {
      if (steep_counter < 10)
      {
        steep_counter++;
        steep();
      } else {
        breakMotors();
      }


    }
  }
  for (int v = 0 ; v <= 1 ; v++)  //підїзд до перешкоди для кращого маневрування
  {
    steep();
    delay(250);
  }
  side_choose = correctside();     // викликаємо функцію вибору сторони обїзду перешкоди
  delay(200);
  breakMotors();
  delay(100);

  if (side_choose == 1)          // 1 - обїзд з правої сторони
  {
    while (side_choose == 1)
    {

      servoa.write(180);
      delay(400);
      int leftcorrect = 0 ;                         // змінна зберігає відстань до першкоди злівої строни після повороту на право
      leftcorrect = Measure2();                     // вимірювання відстані злівої сторони
      servoa.write(90);
      delay(400);
      int forcorrectL =  Measure2();                //вимірювання відстані перед платформою
      delay(300);

      // Наступні інструкції реалізують логіку довороту платформи, якщо першкода яка знаходиться не під прямим кутом до платформи,
      // тобто, якщо виміряні значення вийдуть за допустимі межі, платформа виконає доворот у відповідну сторону.
      if ( forcorrectL < 15 && leftcorrect < 24 )   //якшо відстань попереду більша 15см  і відстань зліва менше 24см поворот на право
      {
        right(150);
        delay(260);
      }
      if (leftcorrect >= 20)                        //відстань зліва більша 20см доворот в ліво
      {
        left(200);
        delay(50);
      }
      if (leftcorrect < 20)                         //відстань менша 20см доворот в право
      {
        right(200);
        delay(50);
      }
      if (leftcorrect > 50)                         // відстань з ліва більша 50см поворот на ліво
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
        int forward_msr1 =  Measure2();
        // Наступні інструкції відповідають за продовження або переривання алгоритму довороту платформи.
        if (forward_msr1 >= 90)                   //якщо після повороту на ліво відстань перед платформою більша 90см 
        {                                         //перервати алгоритм довороту і перейти на початок роботи програми 
          drive = 1 ;                             //для перевірки умови виходу з алгоритму обїзду.
          break;
        }

        if (forward_msr1 < 90)                  //якщо після повороту на ліво відстань перед платформою менша 90см
        {                                       //присвоїти змінній перевірки попереднього повороту 0. 
          drive = 0 ;
        } else drive = 1;
        break;
        int turn_left = 0;                      // змінна для конторлю за відстанню до перешкоди після повороту на ліво
        servoa.write(180);
        delay(100);
        turn_left = Measure2();

        while (turn_left > 28)                 //доворот до перешкоди після повороту у випадку якшо відстань до перешкоди зліва більша 28см
        {
          servoa.write(180);
          delay(100);
          left(80);
          delay(40);
          turn_left = Measure2();
        }

      }
      steep();
      delay(100);
    }


    for (int i = 0 ; i <= 4; i ++)            //цикл для вирівнювання платворми відносно перешкоди зліва після повороту на ліво
    {
      servoa.write(180);
      delay(400);
      int leftcorrect2 = 0 ;
      leftcorrect2 = Measure2();

      if (i == 0 )
      {
        get_Straight = leftcorrect2;        // присвоїти значення першого вимірювання змінній контролю відстані
      }
      delay(100);
      if (leftcorrect2 >= get_Straight)     //відстань більша встанвленої - корекція вліво
      {
        left(200);
        delay(50);
      }
      if (leftcorrect2 < get_Straight)      //відстань менша встановленої - корекція вправо
      {
        right(200);
        delay(50);
      }
      breakMotors();
      delay(100);
      steep();
      delay(100);
    }
  } else if (side_choose == 2 )              // 2 - обїзд з лівої сторони
  {
    while (side_choose == 2)
    {
      servoa.write(0);
      delay(400);
      int rightcorrect = 0 ;              // змінна зберігає відстань до першкоди зправої сторони після повороту на ліво
      rightcorrect = Measure2();
      servoa.write(90);
      delay(400);
      int forcorrectR = Measure2();      //вимірювання відстані перед платформою
      delay(100);
      // Наступні інструкції реалізують логіку довороту платформи, якщо першкода яка знаходиться не під прямим кутом до платформи,
      // тобто, якщо виміряні значення вийдуть за допустимі межі, платформа виконає доворот у відповідну сторону.
      if (forcorrectR < 15 && rightcorrect < 24)        //якшо відстань попереду більша 15см  і відстань зліва менше 24см поворот на ліво
      {
        left(130);
        delay(230);
      }
      if (rightcorrect >= 15)                          //відстань зліва більша 20см доворот в право 
      {
        right(200);
        delay(50);
      }
      if (rightcorrect < 15)                          //відстань менша 20см доворот в ліво
      {
        left(220);
        delay(40);
      }

      if (rightcorrect > 45)                          // відстань з ліва більша 45см поворот на право             
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
        int forward_msr2 =  Measure2();
         // Наступні інструкції відповідають за продовження або переривання алгоритму довороту платформи.
        if (forward_msr2 >= 90)              //якщо після повороту на право відстань перед платформою більша 90см 
        {                                   //перервати алгоритм довороту і перейти на початок роботи програми
          drive = 1 ;                       //для перевірки умови виходу з алгоритму обїзду.
          break;
        }

        if (forward_msr2 < 90)              //якщо після повороту на право відстань перед платформою менша 90см
        {                                   //присвоїти змінній перевірки попереднього повороту 0.
          drive = 0 ;
        } else drive = 1;
        break;
        int turn_right = 0;                 // змінна для конторлю за відстанню до перешкоди після повороту на право
        servoa.write(0);
        delay(100);
        turn_right = Measure2();
        while (turn_right > 28)             //доворот до перешкоди після повороту у випадку якшо відстань до перешкоди зправа більша 28см
        {

          servoa.write(0);
          delay(100);
          right(100);
          delay(50);
          turn_right = Measure2();

        }

      }
      steep();
      delay(100);
    }

    for (int i = 0 ; i <= 4; i ++)              //цикл для вирівнювання платворми відносно перешкоди зправа після повороту на ліво
    {
      servoa.write(180);
      delay(400);
      int rightcorrect2 = 0 ;
      rightcorrect2 = Measure2();

      if (i == 0 )
      {
        get_Straight = rightcorrect2;         // присвоїти значення першого вимірювання змінній контролю відстані
      }
      delay(100);
      if (rightcorrect2 >= get_Straight)      //відстань більша встанвленої - корекція вправо
      {
        right(200);
        delay(50);
      }
      if (rightcorrect2 < get_Straight)      //відстань менша встановленої - корекція вліво
      {
        left(200);
        delay(50);
      }
      breakMotors();
      delay(100);
      steep();
      delay(100);
    }

  }
}
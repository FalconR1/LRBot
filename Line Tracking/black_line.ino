#include <SPI.h>
#include <Servo.h>
Servo ESC1, ESC2, serv4;  //создаем ESC1

int D3 = 3;

// Пины подключения датчиков
#define LEFT_S  A4
#define RIGHT_S A5

// Скорость движения
#define SPEED 100

// Состояния
#define STATE_FORWARD 0
#define STATE_LEFT    1
#define STATE_RIGHT   2

//#define FAST_TIME 500

// Начальные условия
int state = STATE_FORWARD;
//int cur_Speed = SPEED;
//int fastTime = 0;

// Движение по прямой
void m_Forward()
{
  state = STATE_FORWARD;
  ESC1.write(SPEED);
  ESC2.write(SPEED);
  delay(10);

}

// Поворот направо
void m_Right()
{
  state = STATE_RIGHT;


  // Замедляем правое колесо относительно левого
  ESC1.write(90);
  ESC2.write(150);
  delay(10);
}

// Поворот налево
void m_Left()
{

  // Замедляем левое колесо относительно левого
  ESC2.write(90);
  ESC1.write(150);
  delay(10);
}


void setup() {

  ESC1.attach(5);  //подключаем ESC1
  ESC2.attach(6);
//  ESC1.write(1000);  //подключаем ESC1
//  ESC2.write(1000);
//    ESC1.write(1000);  //подключаем ESC1
//  ESC2.write(1000);
  pinMode(A0, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  digitalWrite(D3, HIGH);
  Serial.begin(115200);
  delay(2000);
  void m_Forward();

}

void loop()
{
  int left = analogRead(LEFT_S);
  int right = analogRead(RIGHT_S);
  delay(50);

  // Состояние изменения
  int new_State;

  if (((left < 450) && (right < 450)) || ((left > 450) && (right > 450))) {
    new_State = STATE_FORWARD;
  }

  else if (left > 451) {
    new_State = STATE_LEFT;
  }

  else if (right > 451) {
    new_State = STATE_RIGHT;

  }

  switch (new_State) {
    case STATE_FORWARD:
      m_Forward();
      break;

    case STATE_RIGHT:
      m_Right();
      break;

    case STATE_LEFT:
      m_Left();
      break;
    default:
      break;
  }

}

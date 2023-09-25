#include <Servo.h>

Servo ESC1, ESC2; //создаем ESC1
int Rspeed = 90;
int Lspeed = 90;
int buttonState2 = 0;
int buttonState3 = 0;

void setup() {
pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(2, INPUT);
pinMode(3, INPUT);

ESC1.attach(5,1000,2000); //подключаем ESC1
          //1000 и 2000 это min и max ширина импульса PWM
ESC2.attach(6,1000,2000);    
Serial.begin(115200);
}

void loop() {
  buttonState2 = digitalRead(2);
  if (buttonState2 == HIGH) {
    Rspeed = 0;
    ESC1.write(Rspeed);
    Serial.print("right_rev\n");
  }
  else {
  Rspeed = map(analogRead(A0), 1023, 0, 90, 180);
  ESC1.write(Rspeed);
  }

  buttonState3 = digitalRead(3);
  if (buttonState3 == HIGH) {
    Lspeed = 0;
    ESC2.write(Lspeed);
    Serial.print("left_rev\n");
  }
  else {
  Lspeed = map(analogRead(A1), 1023, 0, 90, 180);
  Serial.print(Lspeed);
  Serial.print("\n");
  ESC2.write(Lspeed);
  }
}

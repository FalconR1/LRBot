#include <PS4BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

//____ПЕРЕМЕННЫЕ____//
Servo ESC1, ESC2;
int speedR = 90;
int speedL = 90;
int battlvl = 0;
//_________________//

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void setup() {
  ESC1.attach(5, 1000, 2000); //подключаем ESC
  ESC2.attach(6, 1000, 2000); //1000 и 2000 это min и max ширина импульса PWM

  Serial.begin(115200);
  #if !defined(__MIPSEL__)
    while (!Serial);
  #endif
  if (Usb.Init() == -1) {
    Serial.println("OSC did not start");
    while (1);
  }
  Serial.println("Waiting for connection");
}

void loop() {
  Usb.Task();
  if (PS4.connected()) {
    PS4.setLed(Red);

    if (PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117) {
      speedR = map(PS4.getAnalogHat(RightHatX), 0, 255, 180, 0); //Развороты
      speedL = map(PS4.getAnalogHat(RightHatX), 0, 255, 0, 180);
      ESC1.write(speedR);
      ESC2.write(speedL);
      PS4.setLed(Yellow);
      PS4.setLedFlash(10, 10);
    }
    else {
      if (PS4.getAnalogHat(LeftHatX) > 137) {
        speedL = map(PS4.getAnalogHat(RightHatX), 138, 255, 91, 180); //Поворот направо
        ESC2.write(speedL);
        PS4.setLed(Yellow);
        PS4.setLedFlash(25, 25);
      }
      else {
        if (PS4.getAnalogHat(LeftHatX) < 117) {
          speedR = map(PS4.getAnalogHat(LeftHatX), 0, 116, 180, 91); //Поворот налево
          ESC1.write(speedR);
          PS4.setLed(Yellow);
          PS4.setLedFlash(25, 25);
        }
        else {
          if (PS4.getAnalogHat(LeftHatX) > 137 && PS4.getAnalogButton(L2)) {
            speedL = map(PS4.getAnalogHat(RightHatX), 138, 255, 89, 0); //Поворот направо задний ход
            ESC2.write(speedL);
            PS4.setLed(Yellow);
            PS4.setLedFlash(25, 25);
          }
          else {
            if (PS4.getAnalogHat(LeftHatX) < 117 && PS4.getAnalogButton(L2)) {
              speedR = map(PS4.getAnalogHat(LeftHatX), 0, 116, 0, 89); //Поворот налево задний ход
              ESC1.write(speedR);
              PS4.setLed(Yellow);
              PS4.setLedFlash(25, 25);
            }
            else {
              if (PS4.getAnalogButton(L2)) {
                speedL = map(PS4.getAnalogButton(L2), 0, 255, 91, 0); //Задний ход
                ESC1.write(speedL);
                ESC2.write(speedL);
                PS4.setLed(Red);
                PS4.setLedFlash(25, 25);
              }
              else {
                if (PS4.getAnalogButton(R2)) {
                  speedR = map(PS4.getAnalogButton(R2), 0, 255, 91, 180); //Движение вперед
                  ESC1.write(speedR);
                  ESC2.write(speedR);
                  PS4.setLed(Red);
                  PS4.setLedFlash(10, 10);
                }
                else {
                  speedR = 90; //Автотормоз
                  speedL = 90;
                  ESC1.write(speedR);
                  ESC2.write(speedL);
                  PS4.setLed(Red);
                  PS4.setLedFlash(0, 0);
                }
              }
            }
          }
        }
      }
    }
      if (PS4.getButtonClick(SHARE)) { //Уровень заряда батареи
      battlvl = PS4.getBatteryLevel();
      Serial.print("Battery ");
      Serial.print(battlvl);
      Serial.println("/15");
      }

      if (PS4.getButtonClick(OPTIONS)) { //Отключение геймпада
      Serial.println("Disconnect");
      PS4.disconnect(); 
    }
  }
}

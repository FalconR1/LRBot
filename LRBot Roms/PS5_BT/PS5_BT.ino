#include <PS5BT.h>
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
int leds = 0;
int speedV = 0;
//_________________//

USB Usb;
BTD Btd(&Usb);
PS5BT PS5(&Btd, PAIR);

//хз че это, но оно надо, какие-то настройки геймпада
bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

void setup() {
  ESC1.attach(5, 1000, 2000); //подключаем ESC
  ESC2.attach(6, 1000, 2000); //1000 и 2000 это min и max ширина импульса PWM

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.println("Waiting for connection");  //это значит что все ок и bluetooth готов
}

void loop() {
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
    lastMessageCounter = PS5.getMessageCounter();
    PS5.setLed(Red);  //цвет LED панели
  }

    if (PS5.getAnalogHat(LeftHatX) > 137) {
      speedL = map(PS5.getAnalogHat(LeftHatX), 138, 255, 91, 180); //Поворот направо
      ESC2.write(speedL);
      PS5.setLed(Yellow);
      //PS5.setLedFlash(25, 25);
    }
    else {
      if (PS5.getAnalogHat(LeftHatX) < 117) {
        speedR = map(PS5.getAnalogHat(LeftHatX), 0, 116, 180, 91); //Поворот налево
        ESC1.write(speedR);
        PS5.setLed(Yellow);
        //PS5.setLedFlash(25, 25); 
      }
      else {
        if (PS5.getAnalogHat(RightHatX) > 137 || PS5.getAnalogHat(RightHatX) < 117) {
          speedR = map(PS5.getAnalogHat(RightHatX), 0, 255, 180, 0); //Развороты
          speedL = map(PS5.getAnalogHat(RightHatX), 0, 255, 0, 180);
          ESC1.write(speedR);
          ESC2.write(speedL);
          PS5.setLed(Yellow);
          //PS5.setLedFlash(10, 10);
        }
        else {
          if (PS5.getButtonPress(L1)) { //Поворот направо задний ход
            ESC2.write(0);
            PS5.setLed(White);
            //PS5.setLedFlash(25, 25);
          }
          else {
            if (PS5.getButtonPress(R1)) { //Поворот налево задний ход
              ESC1.write(0);
              PS5.setLed(White);
              //PS5.setLedFlash(25, 25);
            }
            else {
              if (PS5.getAnalogButton(L2)) {
                speedL = map(PS5.getAnalogButton(L2), 0, 255, 89, 0); //Задний ход
                ESC1.write(speedL);
                ESC2.write(speedL);
                PS5.setLed(Red);
                //PS5.setLedFlash(25, 25);
              }
              else {
                if (PS5.getAnalogButton(R2)) {
                  speedR = map(PS5.getAnalogButton(R2), 0, 255, 91, 150); //Движение вперед
                  ESC1.write(speedR);
                  ESC2.write(speedR);
                  PS5.setLed(Red);
                  /*speedV = speedR / 18 - 5;
                  //Serial.println(speedV);
                  leds = 0;
                  for (int i = 0; i < speedV; i++) {
                    leds = leds << 1 + 1;
                    //Serial.println(leds);
                    PS5.setPlayerLed(leds);
                  }
                  Serial.println(leds);
                
                }*/
                if (speedR >= 102) {
                  PS5.setPlayerLed(1);
                  if (speedR >= 114) {
                    PS5.setPlayerLed(3);
                    if (speedR >= 126) {
                      PS5.setPlayerLed(7);
                      if (speedR >= 135) {
                        PS5.setPlayerLed(15);
                        if (speedR >= 140) {
                          PS5.setPlayerLed(31);
                        }
                      }
                    }
                  }
                }
              }
                else {
                  speedR = 90; //Автотормоз
                  speedL = 90;
                  ESC1.write(speedR);
                  ESC2.write(speedL);
                  PS5.setLed(Red);
                  PS5.setPlayerLedOff();
                  //PS5.setLedFlash(0, 0);
                }
              }
            }
          }
        }
      }
    }
    if (PS5.getButtonClick(OPTIONS)) { //Отключение геймпада
      Serial.println("Disconnect");
      PS5.disconnect(); 
    }

}
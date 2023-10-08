#include <PS4BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

//____ПЕРЕМЕННЫЕ____//
Servo ESC1, ESC2;  //создаем ESC1
int speedR = 90;
int speedL = 90;
int battlvl = 0;
//_________________//

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void setup() {
  ESC1.attach(5, 1000, 2000);  //подключаем ESC1
                              //1000 и 2000 это min и max ширина импульса PWM
  ESC2.attach(6, 1000, 2000);

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

    if (PS5.getAnalogHat(LeftHatX) > 137 || PS5.getAnalogHat(LeftHatX) < 117) { //Подумать поменять одна сторона
      speedR = map(PS5.getAnalogHat(LeftHatX), 0, 255, 180, 0);
      speedL = map(PS5.getAnalogHat(LeftHatX), 0, 255, 0, 180);
      ESC1.write(speedR);
      ESC2.write(speedL);
    }
    else {
      if (PS5.getAnalogHat(RightHatX) > 137 || PS5.getAnalogHat(RightHatX) < 117){
      speedR = map(PS5.getAnalogHat(RightHatX), 0, 255, 180, 0);
      speedL = map(PS5.getAnalogHat(RightHatX), 0, 255, 0, 180);
      ESC1.write(speedR);
      ESC2.write(speedL);
      }
      else {
        if (PS5.getAnalogButton(L2)) {
          speedL = map(PS5.getAnalogButton(L2), 0, 255, 90, 0);
          ESC1.write(speedL);
          ESC2.write(speedL);
        }
        else {
          if (PS5.getAnalogButton(R2)) {
            speedR = map(PS5.getAnalogButton(R2), 0, 255, 90, 180);
            ESC1.write(speedR);
            ESC2.write(speedR);
          }
          else {
            speedR = 90;
            speedL = 90;
            ESC1.write(speedR);
            ESC2.write(speedL);
          }
        }
      }
    }
      if(PS4.getButtonClick(SHARE)){
      battlvl = getBatterylevel();
      Serial.println(battlvl);
      }

      if (PS4.getButtonClick(OPTIONS)) {
      Serial.println("Disconnect");
      PS4.disconnect(); 
    }
  
  }
}

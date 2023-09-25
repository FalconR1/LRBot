#include <PS5BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>


int mode = 0;  //режимы

int start = 0;

int lm = 0;
int light = 0;

Servo ESC1, ESC2, serv4;  //создаем ESC1
int speedR = 90;
int speedL = 90;
int BspeedR = 90;
int BspeedL = 90;
int speedOP = 90;

int s4pos = 0;
//включаем bluetooth и пейрим геймпад
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
  ESC1.attach(10, 1000, 2000);  //подключаем ESC1
                               //1000 и 2000 это min и max ширина импульса PWM
  ESC2.attach(11, 1000, 2000);

  serv4.attach(4);

  pinMode(A0, OUTPUT);

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.print(F("\r\nPS5 Bluetooth Library Started"));  //это значит что все ок и bluetooth готов
}

void loop() {
  Usb.Task();

  if (PS5.connected() && lastMessageCounter != PS5.getMessageCounter()) {
    //Serial.print("Successfully connected\n");
    lastMessageCounter = PS5.getMessageCounter();
    PS5.setLed(255, 102, 0);  //цвет LED панели
  }
  /*else {
    Serial.print("Waiting for connection\n");
    delay(1000);
  }*/

  //JoySticks
  /*if (PS5.getAnalogHat(LeftHatX) > 137 || PS5.getAnalogHat(LeftHatX) < 117 || PS5.getAnalogHat(LeftHatY) > 137 || PS5.getAnalogHat(LeftHatY) < 117 || PS5.getAnalogHat(RightHatX) > 137 || PS5.getAnalogHat(RightHatX) < 117 || PS5.getAnalogHat(RightHatY) > 137 || PS5.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS5.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS5.getAnalogHat(LeftHatY));
      Serial.print(F("\tRightHatX: "));
      Serial.print(PS5.getAnalogHat(RightHatX));
      Serial.print(F("\tRightHatY: "));
      Serial.print(PS5.getAnalogHat(RightHatY));

      speed = map(PS5.getAnalogHat(RightHatY), 0, 255, -255, 255);
      myservo.write(speed);
    }*/
    //Triggers
  /*//Tank
    if (PS5.getAnalogButton(R2)) {
      Serial.print(F("\nR2: "));
      Serial.print(PS5.getAnalogButton(R2));

      speedR = PS5.getAnalogButton(R2);
      speedR = map(PS5.getAnalogButton(R2), 0, 255, 90, 180);
      ESC1.write(speedR);
    } 
    else {
      if (PS5.getButtonPress(R1)) {
        Serial.print("R1\n");
        BspeedR = 0;
        ESC1.write(BspeedR);
      } 
      else {
        BspeedR = 90;
        ESC1.write(BspeedR);
      }
    }
     if (PS5.getAnalogButton(L2)) {
      Serial.print(F("\nL2: "));
      Serial.print(PS5.getAnalogButton(L2));
       speedL = PS5.getAnalogButton(L2);
      speedL = map(PS5.getAnalogButton(L2), 0, 255, 90, 180);
      ESC2.write(speedL);
    } 
    else {
      if (PS5.getButtonPress(L1)) {
        Serial.print("L1\n");
        BspeedL = 0;
        ESC2.write(BspeedL);
      } 
      else {
        BspeedL = 90;
        ESC2.write(BspeedL);
      }
    }*/
  //GTA
  if (PS5.getAnalogButton(R2)) {
          Serial.print(F("\nR2: "));
          Serial.print(PS5.getAnalogButton(R2));

          speedR = PS5.getAnalogButton(R2);
          speedR = map(PS5.getAnalogButton(R2), 0, 255, 90, 180);
          ESC1.write(speedR);
          ESC2.write(speedR);
        }
        else {
          if (PS5.getAnalogButton(L2)) {
          Serial.print(F("\nL2: "));
          Serial.print(PS5.getAnalogButton(L2));

          speedL = PS5.getAnalogButton(L2);
          speedL = map(PS5.getAnalogButton(L2), 0, 255, 90, 0);
          ESC1.write(speedL);
          ESC2.write(speedL);
          }
          else {
            if (PS5.getAnalogHat(LeftHatX) > 137 || PS5.getAnalogHat(LeftHatX) < 117) {
              speedR = PS5.getAnalogHat(LeftHatX);
              speedR = map(PS5.getAnalogHat(LeftHatX), 0, 255, 180, 0);
              speedL = PS5.getAnalogHat(LeftHatX);
              speedL = map(PS5.getAnalogHat(LeftHatX), 0, 255, 0, 180);
              ESC1.write(speedR);
              ESC2.write(speedL);
            }
            else {
              speedR = 90;
              speedL = 90;
              ESC1.write(speedR);
              ESC2.write(speedL);
            }
          }
        }
  
  //Special Buttons
  /*digitalWrite(A0, lm);
  if (PS5.getButtonClick(CREATE)) {
    light++;
    switch (light){
      case 0: lm = 0; Serial.print("0"); break;
      case 1: lm = 0; Serial.print("устраняем ебанутость"); break;
      case 2: lm = 1; Serial.print("1"); break;
      case 3: lm = 0; light = 1; break;
    }
  }

  if (PS5.getButtonClick(OPTIONS)) {
    mode++;
    switch (mode) {
      case 0: PS5.setPlayerLed(0); break;
      case 1: PS5.setPlayerLed(1); break;
      case 2: PS5.setPlayerLed(3); break;
      case 3: PS5.setPlayerLed(7); break;
      case 4: PS5.setPlayerLed(15); break;
      case 5: PS5.setPlayerLed(31); break;
      case 6:
        PS5.setPlayerLed(0);
        mode = 0;
        break;
    }
  }
  }
  /*if (PS5.getButtonClick(PS)){  //mods?
  player_led_mask = (player_led_mask << 1) | 1;
    if (player_led_mask > 0x1F)
      player_led_mask = 0;
      PS5.setPlayerLed(player_led_mask); //The bottom 5 bits set player LEDs
      Serial.print(player_led_mask);
      Serial.print("\n");
}
*/
  //Right Cross
  /*if (PS5.getButtonClick(TRIANGLE)) {
    start = 1;
  }
  if (PS5.getButtonClick(CIRCLE)) {
    //do
  }
  if (PS5.getButtonClick(CROSS)) {
    start = 0;
    //do
  }
  if (PS5.getButtonClick(SQUARE)) {
    //do
  }

  //Left Cross
  if (PS5.getButtonClick(UP)) {
    //do
  }
  if (PS5.getButtonClick(RIGHT)) {
    //do
  }
  if (PS5.getButtonClick(DOWN)) {
    //do
  }
  if (PS5.getButtonClick(LEFT)) {
    //do
  }

}
void line(){
  int l,r;
  l = !digitalRead(LineL);
  r = !digitalRead(LineR);
  if (l == r){
    ESC1.write(140);
    ESC2.write(140);
  }
 else if (l == 1  && r == 0){
    ESC1.write(140);
    ESC2.write(40);
 }
 else if (l == 0  && r == 1){
    ESC1.write(40);
    ESC2.write(140);
 }*/
}
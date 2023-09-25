#include <PS5BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

Servo ESC1, ESC2;

int speedR = 0;
int speedL = 0;

USB Usb;
BTD Btd(&Usb);
PS5BT PS5(&Btd, PAIR);

bool printAngle = false, printTouch = false;
uint16_t lastMessageCounter = -1;
uint8_t player_led_mask = 0;
bool microphone_led = false;
uint32_t ps_timer;

void setup() {
  ESC1.attach(6, 1000, 2000);  //подключаем ESC1
                               //1000 и 2000 это min и max ширина импульса PWM
  ESC2.attach(7, 1000, 2000);

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

  if (PS5.getAnalogButton(R2)) {
    Serial.print(F("\nR2: "));
    Serial.print(PS5.getAnalogButton(R2));
    speedR = PS5.getAnalogButton(R2);
    speedR = map(PS5.getAnalogButton(R2), 0, 255, 0, 180);
    ESC1.write(speedR);
    }
  else {
    speedR = 0;
    ESC1.write(speedR);
  }

  if (PS5.getAnalogButton(L2)) {
    Serial.print(F("\nL2: "));
    Serial.print(PS5.getAnalogButton(L2));
    speedL = PS5.getAnalogButton(L2);
    speedL = map(PS5.getAnalogButton(L2), 0, 255, 0, 180);
    ESC2.write(speedL);
    } 
  else {
    speedL = 0;
    ESC2.write(speedL);
  }
}

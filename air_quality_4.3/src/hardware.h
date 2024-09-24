// hardware.h
#ifndef HARDWARE_H
#define HARDWARE_H

#include <ArduinoJson.h>
#include "debug.h"  // LOG ve LOGLN makroları buradan gelecek

// Fan Kontrol
const int fan_pwmPin = 38;
const int fan_enable = 37;

// PM Sensor
#define RXD1 18
#define TXD1 17
#define LENG 31 // 0x42 + 31 bytes equal to 32 bytes

struct PM {
  int value_01;
  int value_2_5;
  int value_10;
  int pwm;
};

extern unsigned char buf[LENG]; // Değişken bildirimi
extern struct PM pm;            // Değişken bildirimi

void setupHardware();
void motorStart();
void motorStop();
void motorPWM(int pwmValue);
void readPm();

#endif // HARDWARE_H

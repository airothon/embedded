// hardware.cpp
#include "hardware.h"

// Değişken tanımları
unsigned char buf[LENG];
PM pm;


void setupHardware()
{
    Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // PM sensor haberleşmesi
    Serial1.setTimeout(1500);
    pinMode(fan_enable, OUTPUT);    // Motor kontrol çıkışı
    digitalWrite(fan_enable, HIGH); //
}

void motorStart()
{
    LOGLN("motorStart");
    digitalWrite(fan_enable, HIGH);
}

void motorStop()
{
    LOGLN("motorStop");
    digitalWrite(fan_enable, LOW);
}

void motorPWM(int pwmValue)
{
    LOG("motorPWM");
    LOGLN(pwmValue);
    pm.pwm = pwmValue;
    analogWrite(fan_pwmPin, pwmValue);
}

char checkValue(unsigned char *thebuf, char leng)
{
    char receiveflag = 0;
    int receiveSum = 0;

    for (int i = 0; i < (leng - 2); i++)
    {
        receiveSum += thebuf[i];
    }
    receiveSum += 0x42;

    if (receiveSum == ((thebuf[leng - 2] << 8) + thebuf[leng - 1]))
    { // Seri veriyi kontrol et
        receiveflag = 1;
    }
    return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
    return ((thebuf[3] << 8) + thebuf[4]); // PM1.0 değerini hesapla
}

int transmitPM2_5(unsigned char *thebuf)
{
    return ((thebuf[5] << 8) + thebuf[6]); // PM2.5 değerini hesapla
}

int transmitPM10(unsigned char *thebuf)
{
    return ((thebuf[7] << 8) + thebuf[8]); // PM10 değerini hesapla
}

void readPm()
{
    if (Serial1.find(0x42))
    { // Start to read when detect 0x42
        delay(100);
        Serial1.readBytes(buf, LENG);

        if (buf[0] == 0x4d)
        {
            if (checkValue(buf, LENG))
            {
                pm.value_01 = transmitPM01(buf);   // Count PM1.0 value of the air detector module
                pm.value_2_5 = transmitPM2_5(buf); // Count PM2.5 value of the air detector module
                pm.value_10 = transmitPM10(buf);   // Count PM10 value of the air detector module

                LOG("value_01: ");
                LOGLN(pm.value_01);
                LOG("value_2_5: ");
                LOGLN(pm.value_2_5);
                LOG("value_10: ");
                LOGLN(pm.value_10);
            }
        }
    }
}

// myNTP.h
#ifndef MYNTP_H
#define MYNTP_H

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Preferences.h>
#include "debug.h"  // LOG ve LOGLN makroları buradan gelecek


struct MyNTP {
  String serverURL;
  int offset;
};

void setupNTPClient();
void getNtpConfigByEprom();
void saveNtpConfigByEprom();
void removeNtpConfigByEprom();
bool setNtpConfig(String ntpURL, int ntpOffset);
String getDateTime();

MyNTP myNTP;

extern Preferences preferences; // Extern tanım

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String defaultNtpURL = "pool.ntp.org";
int defaultNtpOffset = 0;

void setupNTPClient()
{
    LOGLN("Setting up NTP Client...");
    preferences.begin("ntp-config", true);
    myNTP.serverURL = preferences.getString("ntpURL", defaultNtpURL);
    myNTP.offset = preferences.getInt("ntpOffset", defaultNtpOffset);
    preferences.end();

    LOG("Loaded NTP URL: ");
    LOGLN(myNTP.serverURL);
    LOG("Loaded NTP Offset: ");
    LOGLN(myNTP.offset);

    // NTP client başlatma işlemi
    timeClient.setPoolServerName(myNTP.serverURL.c_str());
    timeClient.setTimeOffset(myNTP.offset * 3600); // Saatleri saniyeye çevir
    timeClient.begin();
}

bool setNtpConfig(String ntpURL, int ntpOffset)
{
    LOGLN("Configuring NTP Client...");
    
    // NTP istemcisini durdur ve yeni yapılandırmayı ayarla
    timeClient.end();
    timeClient.setPoolServerName(ntpURL.c_str());
    timeClient.setTimeOffset(ntpOffset * 3600); // Saatleri saniyeye çevir
    timeClient.begin();
    delay(3000);
    
        myNTP.serverURL = ntpURL;
        myNTP.offset = ntpOffset;
        // Bilgileri kalıcı belleğe kaydet
        saveNtpConfigByEprom();

        LOGLN("NTP config saved to EEPROM.");
        return true;
}

void getNtpConfigByEprom()
{
    LOGLN("Loading NTP config from EEPROM...");
    preferences.begin("ntp-config", true);
    myNTP.serverURL = preferences.getString("ntpURL", defaultNtpURL);
    myNTP.offset = preferences.getInt("ntpOffset", defaultNtpOffset);
    preferences.end();
    LOG("Loaded NTP URL: ");
    LOGLN(myNTP.serverURL);
    LOG("Loaded NTP Offset: ");
    LOGLN(myNTP.offset);

    // NTP client yapılandırmasını yükledikten sonra başlatma işlemi
    timeClient.setPoolServerName(myNTP.serverURL.c_str());
    timeClient.setTimeOffset(myNTP.offset * 3600); // Saatleri saniyeye çevir
    timeClient.begin();
}

void saveNtpConfigByEprom()
{
    LOGLN("Saving NTP config to EEPROM...");
    preferences.begin("ntp-config", false);
    preferences.putString("ntpURL", myNTP.serverURL);
    preferences.putInt("ntpOffset", myNTP.offset);
    preferences.end();
    LOGLN("NTP config saved.");
}

void removeNtpConfigByEprom()
{
    LOGLN("Removing NTP config from EEPROM...");
    preferences.begin("ntp-config", false);
    preferences.remove("ntpURL");
    preferences.remove("ntpOffset");
    preferences.end();
    LOGLN("NTP config removed.");
}

String getDateTime()
{
    LOGLN("Formatting date and time...");
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    char dateTime[25];
    time_t rawTime = (time_t)epochTime;
    struct tm *ptm = localtime(&rawTime);
    snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02d %02d:%02d:%02d",
             ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
             ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    String formattedDateTime = String(dateTime);
    LOG("Formatted DateTime: ");
    LOGLN(formattedDateTime);
    return formattedDateTime;
}

#endif // MYNTP_H

#ifndef MYWIFI_H
#define MYWIFI_H

#include <WiFiUdp.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>

// DEBUG modu için LOG makrosu
// #define DEBUG // Bu satırı kaldırarak DEBUG modunu devre dışı bırakabilirsiniz

#include "debug.h"  // LOG ve LOGLN makroları buradan gelecek


extern Preferences preferences; // Extern tanım

struct WiFiCredentials
{
    String ssid;
    String password;
};

extern WiFiCredentials wifiCredentials;

bool checkInternetConnection();
void loadWiFiConfigFromEEPROM();
void wifiBegin(const char *ssid, String password);
boolean initializeWiFi();
boolean testWiFiConnection();
String scanForWiFiNetworks();
void saveWiFiConfigToEEPROM();
void removeWiFiConfigFromEEPROM();

#endif // MYWIFI_H
#ifndef MYAPI_H
#define MYAPI_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "hardware.h"
#include "debug.h"  // LOG ve LOGLN makroları buradan gelecek


// Supabase API URL ve Anahtarı
extern const char *apiURL;
extern const char *apiKey;


String createPayload(const String date, const PM pm);

void sendRestRequest(const char *url, const String &payload, const char *apiKey);

#endif // MYAPI_H

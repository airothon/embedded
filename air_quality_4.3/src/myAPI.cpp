#include "myAPI.h"
#include "hardware.h"

// Supabase API URL ve Anahtarı
const char *apiURL = "https://gifinihjmwojmnodknuh.supabase.co/rest/v1/ahx";
const char *apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImdpZmluaWhqbXdvam1ub2RrbnVoIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MjI4ODEzMzIsImV4cCI6MjAzODQ1NzMzMn0.Ysb1T3UE4Jn9Hl6pgijKIXoCiZ9nF_-BQKDT2qIKCGE";

// const char *apiURL = "https://vzddycoczdwyucoofvev.supabase.co/rest/v1/hava_kalite";
// const char *apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InZ6ZGR5Y29jemR3eXVjb29mdmV2Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTc0NDQxMDAsImV4cCI6MjAzMzAyMDEwMH0.fXuyxHunJ0Ro-1TeRKdrtOwZ-LLwvSqHc1dlrBWwLZA";


// JSON payload oluşturma fonksiyonu
String createPayload(const String mac, const String date, const PM pm)
{
    StaticJsonDocument<200> doc;
    doc["mac"] = mac;
    doc["date"] = date;
    doc["pm1"] = pm.value_01;
    doc["pm2_5"] = pm.value_2_5;
    doc["pm10"] = pm.value_10;
    doc["fan"] = pm.pwm;

    String output;
    serializeJson(doc, output);
    return output;
}

// REST isteği gönderme fonksiyonu
void sendRestRequest(const char *url, const String &payload, const char *apiKey)
{
    HTTPClient http;
    http.begin(url);

    // Başlık ekleme
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", apiKey);
    http.addHeader("Authorization", "Bearer " + String(apiKey));

    LOG("Sending HTTP POST request...");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        LOG("HTTP Response Code: " + String(httpResponseCode));
        LOG("Response: " + response);
    }
    else
    {
        LOG("HTTP request error: " + String(httpResponseCode));
    }
    http.end();
}

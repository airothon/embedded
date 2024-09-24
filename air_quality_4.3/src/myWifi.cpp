#include "myWifi.h"

WiFiCredentials wifiCredentials; // Değişken burada tanımlanıyor

boolean initializeWiFi()
{
    LOGLN("Initializing WiFi...");
    loadWiFiConfigFromEEPROM();
    if (wifiCredentials.ssid != "")
    {
        LOGLN("Connecting to WiFi...");
        wifiBegin(wifiCredentials.ssid.c_str(), wifiCredentials.password.c_str());
        return true;
    }
    else
    {
        LOGLN("No WiFi credentials found.");
        return false;
    }
}

void wifiBegin(const char *ssid, String password)
{
    LOGLN("Begin WiFi...");
    LOG("ssid:");
    LOGLN(ssid);
    LOG("password:");
    LOGLN(password);

     // Eski bağlantı bilgilerini temizle
    WiFi.disconnect(true);    // Tüm bağlantı bilgilerini temizler
    delay(1000);              // Temizleme işlemi için kısa bir bekleme


    WiFi.begin(ssid, password.c_str());
    delay(1000);              // Temizleme işlemi için kısa bir bekleme

    int attempts = 10; // Deneme sayısını artırdık
    while (WiFi.status() != WL_CONNECTED && attempts > 0)
    {
        WiFi.begin(ssid, password);
        LOG("Attempt: ");
        LOG(10 - attempts + 1);
        LOG(", WiFi status: ");
        LOGLN(WiFi.status());
        delay(500);
        attempts--;
    }

    if (WiFi.status() == WL_CONNECTED) {
        LOGLN("WiFi connected successfully.");
    } else {
        LOGLN("Failed to connect to WiFi.");
    }

    LOG("Final connection status: ");
    LOGLN(WiFi.status());
}

boolean testWiFiConnection()
{
    LOGLN("Starting WiFi connection test...");
    // int attempts = 500;
    // while (WiFi.status() != WL_CONNECTED && attempts > 0)
    // {
    //     LOG(".");
    //     delay(10);
    //     attempts--;
    // }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOGLN("\nWiFi connected successfully.");
        return true;
    }
    LOGLN("\nFailed to connect to WiFi.");
    return false;
}

String scanForWiFiNetworks()
{
    LOGLN("Starting WiFi scan...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    LOG("Number of networks found: ");
    LOGLN(n);

    int attempts = 0;
    while (n == WIFI_SCAN_RUNNING && attempts < 5)
    {
        LOGLN("A scan is already running. Waiting...");
        delay(1000); // Wait for the scan to complete
        n = WiFi.scanNetworks();
        LOG("Number of networks found: ");
        LOGLN(n);
        attempts++;
    }

    if (n == WIFI_SCAN_FAILED)
    {
        LOGLN("WiFi scan failed.");
    }
    else if (n < 0)
    {
        LOG("Unknown error: ");
        LOGLN(n);
    }
    else if (n == 0)
    {
        LOGLN("No networks found.");
    }
    else
    {
        LOG("Networks found: ");
        LOGLN(n);
        String wifiList = "";
        for (int i = 0; i < n; ++i)
        {
            wifiList += WiFi.SSID(i);
            if (i < n - 1)
            {
                wifiList += "\n";
            }
        }
        LOGLN(wifiList);
        return wifiList;
    }
    return "";
}

bool checkInternetConnection()
{
    LOGLN("Checking internet connection...");
    HTTPClient http;
    http.begin("http://clients3.google.com/generate_204"); // This URL is used by Google to check internet connectivity
    int httpCode = http.GET();

    if (httpCode > 0)
    { // Check for the returning code
        LOG("HTTP response code: ");
        LOGLN(httpCode);
        if (httpCode == HTTP_CODE_NO_CONTENT)
        {
            LOGLN("Successfully connected to the internet.");
            http.end(); // Free the resources
            return true; // Successfully connected to the internet
        }
    }
    else
    {
        LOG("HTTP request failed, error: ");
        LOGLN(http.errorToString(httpCode).c_str());
    }

    http.end(); // Free the resources
    LOGLN("Failed to connect to the internet.");
    return false; // Failed to connect to the internet
}

void loadWiFiConfigFromEEPROM()
{
    LOGLN("Loading WiFi config from EEPROM...");
    preferences.begin("wifi-config", true);
    wifiCredentials.ssid = preferences.getString("ssid", "");
    wifiCredentials.password = preferences.getString("password", "");
    preferences.end();
    LOG("Loaded WiFi SSID: ");
    LOGLN(wifiCredentials.ssid);
    LOG("Loaded WiFi Password: ");
    LOGLN(wifiCredentials.password);
}

void saveWiFiConfigToEEPROM()
{
    LOGLN("Saving WiFi config to EEPROM...");
    preferences.begin("wifi-config", false);
    preferences.putString("ssid", wifiCredentials.ssid);
    preferences.putString("password", wifiCredentials.password);
    preferences.end();
    LOGLN("WiFi config saved.");
}

void removeWiFiConfigFromEEPROM()
{
    LOGLN("Removing WiFi config from EEPROM...");
    preferences.begin("wifi-config", false);
    preferences.remove("ssid");
    preferences.remove("password");
    preferences.end();
    LOGLN("WiFi config removed.");
}

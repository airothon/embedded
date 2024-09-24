#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "ui/ui.h"
#include <TimeLib.h>
#include <Ticker.h>
#include "hardware.h"
#include "myNTP.h"
#include "myWifi.h"
#include "myApi.h"
#include <Preferences.h>
#include <WiFi.h>

Preferences preferences;
extern WiFiCredentials wifiCredentials;
extern MyNTP myNTP;
extern PM pm;
// Supabase API URL ve Anahtarı

unsigned long previousMillis = 0;
unsigned long previousMillis_readData = 0;
unsigned long previousMillis_sendData = 0;
unsigned long previousMillis_reConnect = 0;
const long interval = 1000;           // 1 saniye
const long interval_readData = 5000;  // 60 saniye
const long interval_sendData = 60000; // 60 saniye
const long interval_reConnect = 10000; // 10 saniye
const int PmValue1 = 30;
const int PmValue2 = 120;
const int PmValue3 = 200;
String ntpURL;
int ntpOffset;
boolean connectionStatus = false;
char WifiSsid[64];
String Password;

Ticker lvglTicker;
void lvglTick();
void setDropdownWifiSsid(lv_obj_t *);
void setupLVGL();
void check_wifi_connection(lv_timer_t *timer);
void wifi_list(lv_timer_t *timer);
void return_to_main_screen(lv_timer_t *timer);
void return_to_ntp_config_screen(lv_timer_t *timer);
/******************************************************************************/

#define TFT_BL 2
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *lcd = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);
// option 1:
// ILI6485 LCD 480x272
Arduino_RPi_DPI_RGBPanel *lcd = new Arduino_RPi_DPI_RGBPanel(
    bus,
    480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    272 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 7000000 /* prefer_speed */, true /* auto_flush */);

#endif /* !defined(DISPLAY_DEV_KIT) */

/*******************************************************************************
   Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[480 * 272 / 8];
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  lcd->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  lcd->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup()
{
  Serial.begin(115200);
  // Serial.println("Application starting.");
  pinMode(0, OUTPUT); // TOUCH-CS
  setupLVGL();
  lv_timer_handler();
  setupHardware();
  if (initializeWiFi())
  {
    delay(4000);
    if (testWiFiConnection())
    {
      connectionStatus = true;
      lv_obj_set_style_bg_img_recolor(ui_LabelWifi, lv_color_hex(0x0EEE13), LV_PART_MAIN | LV_STATE_DEFAULT);
      // lv_label_set_text(ui_LabelIp, WiFi.localIP().toString().c_str());
      lv_label_set_text(ui_LabelIp, WiFi.macAddress().c_str());

      _ui_screen_change(&ui_ScreenMain, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenMain_screen_init);
      _ui_screen_delete(&ui_ScreenStart);
      lv_timer_handler();
      readPm();
      motorStart();
      
    }
    else
    {
      connectionStatus = false;
      // Serial.println("No WiFi connection!");
      lv_timer_handler();
      setDropdownWifiSsid(ui_ScreenStart);
    }
  }
  else
  {
    connectionStatus = false;
    // Serial.println("No WiFi connection!");
    lv_timer_handler();
    setDropdownWifiSsid(ui_ScreenStart);
  }
  
  setupNTPClient();

}

void printScreen()
{

  if (lv_scr_act() == ui_ScreenStart)
  {
    // Serial.println("Active ui_ScreenInfo ");
  }
  else if (lv_scr_act() == ui_ScreenMain)
  {
    // Serial.println("Active ui_ScreenMain ");
  }
  else if (lv_scr_act() == ui_ScreenNtpConf)
  {
    // Serial.println("Active ui_ScreenMain ");
  }
  else if (lv_scr_act() == ui_ScreenInfo)
  {
    // Serial.println("Active ui_ScreenMain ");
  }
}

void loop()
{
  delay(5);
  lv_timer_handler();
  unsigned long currentMillis = millis();
  if (lv_scr_act() == ui_ScreenMain && currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    if (testWiFiConnection() && connectionStatus)
    {
      String currentDateTime = getDateTime();
      int spaceIndex = currentDateTime.indexOf(' ');
      String currentDate = currentDateTime.substring(0, spaceIndex);
      String currentTime = currentDateTime.substring(spaceIndex + 1);
      lv_label_set_text(ui_LabelTime, currentTime.c_str());
      lv_label_set_text(ui_LabelDate, currentDate.c_str());

      if (currentMillis - previousMillis_readData >= interval_readData)
      {
        previousMillis_readData = currentMillis;
        lv_obj_set_style_bg_img_recolor(ui_LabelWifi, lv_color_hex(0x0EEE13), LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_label_set_text(ui_LabelIp, WiFi.localIP().toString().c_str());
        lv_label_set_text(ui_LabelIp, WiFi.macAddress().c_str());

        motorStart();

        readPm();
        // // Serial.print("PM1.0: ");
        // // Serial.print(pm.value_01);
        // // Serial.println("  ug/m3");

        // // Serial.print("PM2.5: ");
        // // Serial.print(pm.value_2_5);
        // // Serial.println("  ug/m3");

        // // Serial.print("PM10: ");
        // // Serial.print(pm.value_10);
        // // Serial.println("  ug/m3");

        // // Serial.print("PWM: ");
        // // Serial.print(pm.pwm);
        // // Serial.println("");
        // // Serial.println();
        if (pm.value_2_5 < PmValue1)
        {
          lv_arc_set_value(ui_ArcFan, 1);
          motorPWM(25);
          lv_label_set_text(ui_LabelFan, ("% 10"));
          lv_obj_set_style_arc_color(ui_ArcFan, lv_color_hex(0x00FF00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(ui_ArcFan, lv_color_hex(0x00FF00), LV_PART_KNOB | LV_STATE_DEFAULT);
          // yesil
        }
        else if (pm.value_2_5 < PmValue2)
        {
          lv_arc_set_value(ui_ArcFan, 3);
          motorPWM(75);
          lv_label_set_text(ui_LabelFan, ("% 30"));
          lv_obj_set_style_arc_color(ui_ArcFan, lv_color_hex(0xDDE809), LV_PART_INDICATOR | LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(ui_ArcFan, lv_color_hex(0xDDE809), LV_PART_KNOB | LV_STATE_DEFAULT);
          // sarı
        }
        else if (pm.value_2_5 < PmValue3)
        {
          lv_arc_set_value(ui_ArcFan, 6);
          motorPWM(150);
          lv_label_set_text(ui_LabelFan, ("% 60"));
          lv_obj_set_style_arc_color(ui_ArcFan, lv_color_hex(0xFF7F00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(ui_ArcFan, lv_color_hex(0xFF7F00), LV_PART_KNOB | LV_STATE_DEFAULT);
          // sarı
        }
        else
        {
          if (pm.value_2_5 > 1000)
            pm.value_2_5 = 1000;
          lv_arc_set_value(ui_ArcFan, 9);
          motorPWM(255);
          lv_label_set_text(ui_LabelFan, ("% 100"));
          lv_obj_set_style_arc_color(ui_ArcFan, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(ui_ArcFan, lv_color_hex(0xFF0000), LV_PART_KNOB | LV_STATE_DEFAULT);
          // kırmızı
        }

          lv_label_set_text(ui_LabelPm, (String(pm.value_2_5) + " ug/m3").c_str());
           if (!checkInternetConnection())
              connectionStatus = false;
      }
      if (currentMillis - previousMillis_sendData >= interval_sendData)
      {
          previousMillis_sendData = currentMillis;
          // String payload = createPayload(getDateTime(), pm);
          String payload = createPayload(WiFi.macAddress(), getDateTime(), pm);
          sendRestRequest(apiURL, payload, apiKey);
      }
    }
    else
    {
      motorStop();
      lv_label_set_text(ui_LabelPm, "0 um/ml");
      lv_arc_set_value(ui_ArcFan, 0);
      lv_label_set_text(ui_LabelFan, "%0");
      lv_obj_set_style_arc_color(ui_ArcFan, lv_color_hex(0xA1A1B1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_ArcFan, lv_color_hex(0xA1A1B1), LV_PART_KNOB | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_img_recolor(ui_LabelWifi, lv_color_hex(0xFF4041), LV_PART_MAIN | LV_STATE_DEFAULT); // red icon
      lv_label_set_text(ui_LabelIp, "0.0.0.0");

       if (currentMillis - previousMillis_reConnect >= interval_reConnect)
      {
        previousMillis_reConnect = currentMillis;
        WiFi.reconnect();
      }
      if (checkInternetConnection())
          connectionStatus = true;
    }
  }
}

void setupLVGL()
{
  // lvgl初始化
  lv_init();

  // Init Display
  lcd->begin();
  lcd->setTextSize(2);
  lcd->fillScreen(BLACK);

  // 触摸初始化
  touch_init();
  // setTouch(calData);
  screenWidth = lcd->width();
  screenHeight = lcd->height();
  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 8);

  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  delay(300);
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif
  ui_init();
  lv_timer_handler();
}

void lvglTick()
{
  // Serial.println("timmer handler");
  lv_timer_handler(); // LVGL zamanlayıcı işleyicisini çağır
}

void setDropdownWifiSsid(lv_obj_t *delete_screen)
{
  // Serial.println("setDropdownWifiSsid start");

  // Kullanıcıya tarama işlemi hakkında bilgi ver
  lv_label_set_text(ui_LabelInfo, "Network is scanning, please wait...");
  _ui_screen_change(&ui_ScreenInfo, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenInfo_screen_init);
  _ui_screen_delete(&ui_ScreenMain);
  delay(1000);
  lv_timer_t *check_timer = lv_timer_create(wifi_list, 1500, NULL);
  lv_timer_set_repeat_count(check_timer, 1); // 1 kez dene

  // Confirm the active screen change
  // Serial.println("setDropdownWifiSsid end");
}

void wifi_list(lv_timer_t *timer)
{
  // Serial.println("wifi_list start");
  String wifiList = scanForWiFiNetworks();
  lv_dropdown_set_options(ui_DropdownWifiSsid, wifiList.c_str());

  if (!wifiCredentials.ssid.isEmpty())
  {
    lv_textarea_set_text(ui_TextAreaWifiPass, wifiCredentials.password.c_str());

    // Get the list of SSIDs from the dropdown
    const char *options = lv_dropdown_get_options(ui_DropdownWifiSsid);

    // Split the SSID list into individual SSIDs
    String ssidString(options);
    int startIndex = 0;
    int endIndex = ssidString.indexOf('\n');
    int optionIndex = 0;
    bool ssidFound = false;

    while (endIndex != -1)
    {
      String ssid = ssidString.substring(startIndex, endIndex);

      if (ssid == wifiCredentials.ssid)
      {
        lv_dropdown_set_selected(ui_DropdownWifiSsid, optionIndex);
        ssidFound = true;
        break;
      }

      startIndex = endIndex + 1;
      endIndex = ssidString.indexOf('\n', startIndex);
      optionIndex++;
    }

    if (!ssidFound)
    {
      // Check the last SSID if it was not followed by a newline character
      String ssid = ssidString.substring(startIndex);
      if (ssid == wifiCredentials.ssid)
      {
        lv_dropdown_set_selected(ui_DropdownWifiSsid, optionIndex);
      }
    }
  }

  lv_timer_t *wait_timer = lv_timer_create([](lv_timer_t *timer)
                                           {
                                             // Bu fonksiyon 3 saniye sonra çalışacak

                                             _ui_screen_change(&ui_ScreenWifiConf, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenWifiConf_screen_init);
                                             _ui_screen_delete(&ui_ScreenInfo);
                                             lv_timer_del(timer); // Zamanlayiciyi sil
                                           },
                                           1000, NULL);
  // Fonksiyonu hemen çalıştırmamak için timer başlatıyoruz
  lv_timer_set_repeat_count(wait_timer, 1); // Timer bir kez çalışacak
  // Serial.println("wifi_list end");
}

void call_screen_wifi_config(lv_event_t *e)
{
  // Serial.println("call_screen_wifi_config start");
  setDropdownWifiSsid(ui_ScreenMain);
  // Serial.println("call_screen_wifi_config end");
}

void save_wifi_config(lv_event_t *e)
{
  // Serial.println("save_wifi_config");

  

  // WiFi'ye bağlanmayı dene

  // Serial.println("check_wifi_connection ");
  
  lv_label_set_text(ui_LabelInfo, "Connecting to WiFi...");
  _ui_screen_change(&ui_ScreenInfo, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenInfo_screen_init);
  _ui_screen_delete(&ui_ScreenWifiConf);

  // Bağlantı durumunu kontrol etmek için bir zamanlayıcı oluştur
  lv_timer_t *check_timer = lv_timer_create(check_wifi_connection, 1500, NULL);
  lv_timer_set_repeat_count(check_timer, 1); // 1 kez dene
  
}

void check_wifi_connection(lv_timer_t *timer)
{
  lv_dropdown_get_selected_str(ui_DropdownWifiSsid, WifiSsid, sizeof(WifiSsid));
  Password = lv_textarea_get_text(ui_TextAreaWifiPass);

  // Serial.print("WifiSsid: ");
  // Serial.println(WifiSsid);
  // Serial.print("WifiPass: ");
  // Serial.println(Password);

    // WiFi.begin(WifiSsid, Password.c_str());
    // delay(3000);


  wifiBegin(WifiSsid, Password);

  if (testWiFiConnection())
  {
    connectionStatus = true;
    lv_label_set_text(ui_LabelInfo, "WiFi connection successful!");
    wifiCredentials.ssid = WifiSsid;
    wifiCredentials.password = Password;
    saveWiFiConfigToEEPROM();
    lv_timer_t *wait_timer = lv_timer_create([](lv_timer_t *timer)
                                             {
        // Bu fonksiyon 3 saniye sonra çalışacak
         
      lv_obj_set_style_bg_img_recolor(ui_LabelWifi, lv_color_hex(0x0EEE13), LV_PART_MAIN | LV_STATE_DEFAULT);
      // lv_label_set_text(ui_LabelIp, WiFi.localIP().toString().c_str());
      lv_label_set_text(ui_LabelIp, WiFi.macAddress().c_str());

      _ui_screen_change(&ui_ScreenMain, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenMain_screen_init);
      _ui_screen_delete(&ui_ScreenInfo); }, 3000, NULL);
    lv_timer_del(timer);

    // Fonksiyonu hemen çalıştırmamak için timer başlatıyoruz
    lv_timer_set_repeat_count(wait_timer, 1); // Timer bir kez çalışacak
  }
  else
  {
    connectionStatus = false;
    lv_label_set_text(ui_LabelInfo, "WiFi connection failed.");
    lv_timer_t *wait_timer = lv_timer_create([](lv_timer_t *timer)
                                             {
                                               // Bu fonksiyon 3 saniye sonra çalışacak

                                               _ui_screen_change(&ui_ScreenWifiConf, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenWifiConf_screen_init);
                                               _ui_screen_delete(&ui_ScreenInfo);
                                               lv_timer_del(timer); // Zamanlayiciyi sil
                                             },
                                             3000, NULL);

    // Fonksiyonu hemen çalıştırmamak için timer başlatıyoruz
    lv_timer_set_repeat_count(wait_timer, 1); // Timer bir kez çalışacak
  }
  // Serial.println("check_wifi_connection end");
}

void refresh_wifi_list(lv_event_t *e)
{
  setDropdownWifiSsid(ui_ScreenWifiConf);
}

void show_info_message(lv_timer_t *timer)
{
  if (setNtpConfig(ntpURL, ntpOffset))
  {
    lv_label_set_text(ui_LabelInfo, "Registration successful.");
    lv_timer_t *main_screen_timer = lv_timer_create(return_to_main_screen, 4000, NULL);
    lv_timer_set_repeat_count(main_screen_timer, 1);
  }
  else
  {
    lv_label_set_text(ui_LabelInfo, "Registration unsuccessful.");
    lv_timer_t *main_screen_timer = lv_timer_create(return_to_ntp_config_screen, 4000, NULL);
    lv_timer_set_repeat_count(main_screen_timer, 1);
  }
}

void return_to_main_screen(lv_timer_t *timer)
{
  _ui_screen_change(&ui_ScreenMain, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenMain_screen_init);
  _ui_screen_delete(&ui_ScreenInfo);
  // Serial.println("set_ScreenMain");
}

void return_to_ntp_config_screen(lv_timer_t *timer)
{
  _ui_screen_change(&ui_ScreenNtpConf, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenNtpConf_screen_init);
  _ui_screen_delete(&ui_ScreenInfo);
  // Serial.println("set_ScreenNtpConfig");
}

void call_screen_ntp_config(lv_event_t *e)
{
  lv_textarea_set_text(ui_TextAreaNtp, myNTP.serverURL.c_str());
  String options = "-6\n-5\n-4\n-3\n-2\n-1\n0\n+1\n+2\n+3\n+4\n+5\n+6";
  lv_dropdown_set_options(ui_DropdownNtp, options.c_str());

  // Kaydedilmiş ofset değerinin indeksini bul
  int optionIndex = 6 + myNTP.offset; // -6 ofseti indeks 0'a karşılık gelir, bu yüzden 6 ekliyoruz

  lv_dropdown_set_selected(ui_DropdownNtp, optionIndex); // Default selected offset
}

void save_ntp_config(lv_event_t *e)
{
  // Serial.println("save_ntp_config");
  char NtpOffset[32];
  lv_dropdown_get_selected_str(ui_DropdownNtp, NtpOffset, sizeof(NtpOffset));
  ntpURL = lv_textarea_get_text(ui_TextAreaNtp);
  ntpOffset = atoi(NtpOffset); // Convert NtpOffset to integer

  // Serial.println("set_ScreenInfo");
  lv_label_set_text(ui_LabelInfo, "NTP settings updated.");
  _ui_screen_change(&ui_ScreenInfo, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenInfo_screen_init);
  _ui_screen_delete(&ui_ScreenNtpConf);

  // Serial.print("NtpURL: ");
  // Serial.println(ntpURL);
  // Serial.print("NtpOffset: ");
  // Serial.println(ntpOffset);

  delay(1000);
  lv_timer_t *info_timer = lv_timer_create(show_info_message, 1000, NULL);
  lv_timer_set_repeat_count(info_timer, 1);
}

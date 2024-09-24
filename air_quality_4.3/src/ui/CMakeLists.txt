SET(SOURCES ui_ScreenStart.c
    ui_ScreenMain.c
    ui_ScreenNtpConf.c
    ui_ScreenWifiConf.c
    ui_ScreenInfo.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_1537150334.c
    ui_img_wifi_png.c
    ui_img_1215880664.c
    ui_img_schedule_png.c
    ui_img_357916539.c
    ui_img_534919753.c)

add_library(ui ${SOURCES})

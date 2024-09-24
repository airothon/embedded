// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: air_quality

#include "ui.h"

void ui_ScreenNtpConf_screen_init(void)
{
ui_ScreenNtpConf = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenNtpConf, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_TextAreaNtp = lv_textarea_create(ui_ScreenNtpConf);
lv_obj_set_width( ui_TextAreaNtp, 336);
lv_obj_set_height( ui_TextAreaNtp, LV_SIZE_CONTENT);   /// 40
lv_obj_set_x( ui_TextAreaNtp, -62 );
lv_obj_set_y( ui_TextAreaNtp, -101 );
lv_obj_set_align( ui_TextAreaNtp, LV_ALIGN_CENTER );
lv_textarea_set_text(ui_TextAreaNtp,"pool.ntp.org");
lv_textarea_set_placeholder_text(ui_TextAreaNtp,"pool.ntp.org");
lv_textarea_set_one_line(ui_TextAreaNtp,true);



ui_DropdownNtp = lv_dropdown_create(ui_ScreenNtpConf);
lv_dropdown_set_options( ui_DropdownNtp, "0\n+1\n+2\n+3\n+4\n+5\n+6" );
lv_obj_set_width( ui_DropdownNtp, 74);
lv_obj_set_height( ui_DropdownNtp, LV_SIZE_CONTENT);   /// 12
lv_obj_set_x( ui_DropdownNtp, 150 );
lv_obj_set_y( ui_DropdownNtp, -103 );
lv_obj_set_align( ui_DropdownNtp, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_DropdownNtp, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags



ui_Keyboard2 = lv_keyboard_create(ui_ScreenNtpConf);
lv_obj_set_width( ui_Keyboard2, 466);
lv_obj_set_height( ui_Keyboard2, 157);
lv_obj_set_x( ui_Keyboard2, -2 );
lv_obj_set_y( ui_Keyboard2, 53 );
lv_obj_set_align( ui_Keyboard2, LV_ALIGN_CENTER );

ui_Button2 = lv_btn_create(ui_ScreenNtpConf);
lv_obj_set_width( ui_Button2, 120);
lv_obj_set_height( ui_Button2, 30);
lv_obj_set_x( ui_Button2, -169 );
lv_obj_set_y( ui_Button2, -54 );
lv_obj_set_align( ui_Button2, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xEFDD00), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label4 = lv_label_create(ui_Button2);
lv_obj_set_width( ui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label4, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label4,"Back");

ui_Button1 = lv_btn_create(ui_ScreenNtpConf);
lv_obj_set_width( ui_Button1, 120);
lv_obj_set_height( ui_Button1, 30);
lv_obj_set_x( ui_Button1, 42 );
lv_obj_set_y( ui_Button1, -54 );
lv_obj_set_align( ui_Button1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Label10 = lv_label_create(ui_Button1);
lv_obj_set_width( ui_Label10, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label10, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label10, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label10,"Save");

lv_keyboard_set_textarea(ui_Keyboard2,ui_TextAreaNtp);
lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);

}

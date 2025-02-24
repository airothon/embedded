// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: air_quality

#include "ui.h"

void ui_ScreenStart_screen_init(void)
{
ui_ScreenStart = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenStart, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_ScreenStart,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_ScreenStart, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

ui_Label2 = lv_label_create(ui_ScreenStart);
lv_obj_set_width( ui_Label2, 180);
lv_obj_set_height( ui_Label2, 124);
lv_obj_set_x( ui_Label2, -1 );
lv_obj_set_y( ui_Label2, -75 );
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,"");
lv_obj_set_style_bg_img_src( ui_Label2, &ui_img_1537150334, LV_PART_MAIN | LV_STATE_DEFAULT );

ui_Label11 = lv_label_create(ui_ScreenStart);
lv_obj_set_width( ui_Label11, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label11, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label11, 0 );
lv_obj_set_y( ui_Label11, 49 );
lv_obj_set_align( ui_Label11, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label11,"Application is starting.");
lv_obj_set_style_text_font(ui_Label11, &lv_font_montserrat_24, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label5 = lv_label_create(ui_ScreenStart);
lv_obj_set_width( ui_Label5, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label5, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label5, -5 );
lv_obj_set_y( ui_Label5, 86 );
lv_obj_set_align( ui_Label5, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label5,"Please Wait.");
lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

}

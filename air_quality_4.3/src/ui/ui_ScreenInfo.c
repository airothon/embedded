// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.1
// LVGL version: 8.3.11
// Project name: air_quality

#include "ui.h"

void ui_ScreenInfo_screen_init(void)
{
ui_ScreenInfo = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenInfo, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_ScreenInfo,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_ScreenInfo, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

ui_LabelInfo = lv_label_create(ui_ScreenInfo);
lv_obj_set_width( ui_LabelInfo, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_LabelInfo, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_LabelInfo, LV_ALIGN_CENTER );
lv_label_set_text(ui_LabelInfo,"");
lv_obj_set_style_text_color(ui_LabelInfo, lv_color_hex(0x2D1CE2), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_LabelInfo, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_LabelInfo, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

}

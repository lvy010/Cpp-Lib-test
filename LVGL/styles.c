#include "lvgl/lvgl.h"
#include <stdio.h>

// 外部样式变量声明
extern lv_style_t my_button_style;
extern lv_style_t my_button_pressed_style;
extern lv_style_t knob_style;
extern lv_style_t global_font_style;

// 样式设置函数
void setup_styles() {
    printf("正在初始化样式...\n");
    
    // 1. 基础按钮样式（蓝色背景，圆角）
    lv_style_init(&my_button_style);
    lv_style_set_bg_color(&my_button_style, lv_color_hex(0x007BFF));
    lv_style_set_bg_opa(&my_button_style, LV_OPA_COVER);
    lv_style_set_radius(&my_button_style, 10);
    lv_style_set_pad_all(&my_button_style, 10);
    
    // 2. 按钮按压状态样式（深蓝色）
    lv_style_init(&my_button_pressed_style);
    lv_style_set_bg_color(&my_button_pressed_style, lv_color_hex(0x0056b3));
    lv_style_set_radius(&my_button_pressed_style, 10);
    
    // 3. 滑块旋钮样式（红色圆形）
    lv_style_init(&knob_style);
    lv_style_set_bg_color(&knob_style, lv_color_hex(0xFF0000));
    lv_style_set_radius(&knob_style, LV_RADIUS_CIRCLE);
    
    // 4. 全局字体样式
    lv_style_init(&global_font_style);
    lv_style_set_text_color(&global_font_style, lv_color_black());
    lv_style_set_text_font(&global_font_style, &lv_font_montserrat_16);
    
    printf("样式初始化完成！\n");
}

// 创建界面函数
void create_ui() {
    printf("正在创建界面...\n");
    
    // 创建主屏幕
    lv_obj_t * screen_main = lv_obj_create(NULL);
    lv_screen_load(screen_main);
    
    // 应用全局字体样式
    lv_obj_add_style(screen_main, &global_font_style, 0);
    
    // 1. 创建标题
    lv_obj_t * title_label = lv_label_create(screen_main);
    lv_label_set_text(title_label, "LVGL 样式测试");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
    
    // 2. 创建基础按钮
    lv_obj_t * button1 = lv_btn_create(screen_main);
    lv_obj_set_size(button1, 120, 50);
    lv_obj_set_pos(button1, 50, 80);
    
    // 应用样式
    lv_obj_add_style(button1, &my_button_style, 0);
    lv_obj_add_style(button1, &my_button_pressed_style, LV_STATE_PRESSED);
    
    lv_obj_t * button1_label = lv_label_create(button1);
    lv_label_set_text(button1_label, "按钮1");
    lv_obj_align(button1_label, LV_ALIGN_CENTER, 0, 0);
    
    // 3. 创建第二个按钮（红色变体）
    lv_obj_t * button2 = lv_btn_create(screen_main);
    lv_obj_set_size(button2, 120, 50);
    lv_obj_set_pos(button2, 200, 80);
    
    // 应用基础样式
    lv_obj_add_style(button2, &my_button_style, 0);
    lv_obj_add_style(button2, &my_button_pressed_style, LV_STATE_PRESSED);
    
    // 添加红色变体样式（层叠样式示例）
    static lv_style_t red_button_style;
    lv_style_init(&red_button_style);
    lv_style_set_bg_color(&red_button_style, lv_color_hex(0xFF0000));
    lv_obj_add_style(button2, &red_button_style, 0);
    
    lv_obj_t * button2_label = lv_label_create(button2);
    lv_label_set_text(button2_label, "按钮2");
    lv_obj_align(button2_label, LV_ALIGN_CENTER, 0, 0);
    
    // 4. 创建滑块（展示部件组成单元）
    lv_obj_t * slider = lv_slider_create(screen_main);
    lv_obj_set_size(slider, 200, 20);
    lv_obj_set_pos(slider, 50, 160);
    
    // 为滑块旋钮应用独立样式
    lv_obj_add_style(slider, &knob_style, LV_PART_KNOB);
    
    // 5. 创建进度条
    lv_obj_t * bar = lv_bar_create(screen_main);
    lv_obj_set_size(bar, 200, 20);
    lv_obj_set_pos(bar, 50, 200);
    lv_bar_set_value(bar, 60, LV_ANIM_ON);
    
    // 6. 创建本地样式示例（橙色按钮）
    lv_obj_t * button3 = lv_btn_create(screen_main);
    lv_obj_set_size(button3, 120, 50);
    lv_obj_set_pos(button3, 350, 80);
    
    // 使用本地样式设置
    lv_obj_set_style_bg_color(button3, lv_color_make(255, 128, 0), 0); // 橙色
    lv_obj_set_style_radius(button3, 20, 0); // 20px 圆角
    
    lv_obj_t * button3_label = lv_label_create(button3);
    lv_label_set_text(button3_label, "本地样式");
    lv_obj_align(button3_label, LV_ALIGN_CENTER, 0, 0);
    
    // 7. 创建说明文本
    lv_obj_t * info_label = lv_label_create(screen_main);
    lv_label_set_text(info_label, "样式测试说明：\n"
                                  "• 按钮1：基础蓝色样式\n"
                                  "• 按钮2：红色变体样式\n"
                                  "• 按钮3：本地样式（橙色）\n"
                                  "• 滑块：红色圆形旋钮\n"
                                  "• 进度条：默认样式");
    lv_obj_set_pos(info_label, 50, 250);
    
    printf("界面创建完成！\n");
} 
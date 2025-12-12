#include "lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// 全局样式变量
static lv_style_t my_button_style;
static lv_style_t my_button_pressed_style;
static lv_style_t red_button_style;
static lv_style_t orange_button_style;
static lv_style_t knob_style;
static lv_style_t global_font_style;

// 模拟显示缓冲区
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[800 * 480 / 10];
static lv_color_t buf2[800 * 480 / 10];

// 模拟显示驱动刷新回调
static void disp_flush_cb(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    // 在真实环境中，这里会将颜色数据发送到显示器
    // 在模拟环境中，我们只是打印一些信息
    printf("显示刷新: 区域 (%d,%d) -> (%d,%d)\n", 
           area->x1, area->y1, area->x2, area->y2);
    
    // 通知 LVGL 刷新完成
    lv_disp_flush_ready(disp);
}

// 模拟输入设备读取回调
static void input_read_cb(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // 模拟触摸输入（这里只是示例）
    static int touch_x = 0, touch_y = 0;
    static int touch_state = 0;
    
    // 模拟触摸事件
    if (touch_state == 0) {
        touch_x = 100 + (rand() % 200);
        touch_y = 100 + (rand() % 200);
        touch_state = 1;
    } else {
        touch_state = 0;
    }
    
    data->point.x = touch_x;
    data->point.y = touch_y;
    data->state = touch_state ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

// 按钮事件回调
static void button_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    
    if(code == LV_EVENT_CLICKED) {
        printf("按钮被点击！\n");
        
        // 获取按钮文本
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        if(label) {
            const char * text = lv_label_get_text(label);
            printf("按钮文本: %s\n", text);
        }
    }
}

// 滑块事件回调
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    printf("滑块值: %d\n", value);
}

// 初始化样式
void setup_styles() {
    printf("正在初始化 LVGL 样式...\n");
    
    // 1. 基础按钮样式（蓝色背景，圆角）
    lv_style_init(&my_button_style);
    lv_style_set_bg_color(&my_button_style, lv_color_hex(0x007BFF));
    lv_style_set_bg_opa(&my_button_style, LV_OPA_COVER);
    lv_style_set_radius(&my_button_style, 10);
    lv_style_set_pad_all(&my_button_style, 10);
    lv_style_set_text_color(&my_button_style, lv_color_white());
    
    // 2. 按钮按压状态样式（深蓝色）
    lv_style_init(&my_button_pressed_style);
    lv_style_set_bg_color(&my_button_pressed_style, lv_color_hex(0x0056b3));
    lv_style_set_radius(&my_button_pressed_style, 10);
    lv_style_set_text_color(&my_button_pressed_style, lv_color_white());
    
    // 3. 红色按钮样式
    lv_style_init(&red_button_style);
    lv_style_set_bg_color(&red_button_style, lv_color_hex(0xFF0000));
    lv_style_set_radius(&red_button_style, 10);
    lv_style_set_pad_all(&red_button_style, 10);
    lv_style_set_text_color(&red_button_style, lv_color_white());
    
    // 4. 橙色按钮样式
    lv_style_init(&orange_button_style);
    lv_style_set_bg_color(&orange_button_style, lv_color_hex(0xFF8000));
    lv_style_set_radius(&orange_button_style, 20);
    lv_style_set_pad_all(&orange_button_style, 10);
    lv_style_set_text_color(&orange_button_style, lv_color_white());
    
    // 5. 滑块旋钮样式（红色圆形）
    lv_style_init(&knob_style);
    lv_style_set_bg_color(&knob_style, lv_color_hex(0xFF0000));
    lv_style_set_radius(&knob_style, LV_RADIUS_CIRCLE);
    
    // 6. 全局字体样式
    lv_style_init(&global_font_style);
    lv_style_set_text_color(&global_font_style, lv_color_black());
    
    printf("✅ 样式初始化完成！\n");
}

// 创建界面
void create_ui() {
    printf("正在创建 LVGL 界面...\n");
    
    // 创建主屏幕
    lv_obj_t * screen_main = lv_obj_create(NULL);
    lv_screen_load(screen_main);
    
    // 应用全局字体样式
    lv_obj_add_style(screen_main, &global_font_style, 0);
    
    // 1. 创建标题
    lv_obj_t * title_label = lv_label_create(screen_main);
    lv_label_set_text(title_label, "LVGL 完整样式测试");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
    
    // 2. 创建基础按钮
    lv_obj_t * button1 = lv_btn_create(screen_main);
    lv_obj_set_size(button1, 120, 50);
    lv_obj_set_pos(button1, 50, 80);
    
    // 应用样式
    lv_obj_add_style(button1, &my_button_style, 0);
    lv_obj_add_style(button1, &my_button_pressed_style, LV_STATE_PRESSED);
    
    // 添加事件回调
    lv_obj_add_event_cb(button1, button_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t * button1_label = lv_label_create(button1);
    lv_label_set_text(button1_label, "按钮1");
    lv_obj_align(button1_label, LV_ALIGN_CENTER, 0, 0);
    
    // 3. 创建红色按钮
    lv_obj_t * button2 = lv_btn_create(screen_main);
    lv_obj_set_size(button2, 120, 50);
    lv_obj_set_pos(button2, 200, 80);
    
    // 应用红色样式
    lv_obj_add_style(button2, &red_button_style, 0);
    lv_obj_add_event_cb(button2, button_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t * button2_label = lv_label_create(button2);
    lv_label_set_text(button2_label, "按钮2");
    lv_obj_align(button2_label, LV_ALIGN_CENTER, 0, 0);
    
    // 4. 创建橙色按钮
    lv_obj_t * button3 = lv_btn_create(screen_main);
    lv_obj_set_size(button3, 120, 50);
    lv_obj_set_pos(button3, 350, 80);
    
    // 应用橙色样式
    lv_obj_add_style(button3, &orange_button_style, 0);
    lv_obj_add_event_cb(button3, button_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t * button3_label = lv_label_create(button3);
    lv_label_set_text(button3_label, "按钮3");
    lv_obj_align(button3_label, LV_ALIGN_CENTER, 0, 0);
    
    // 5. 创建滑块
    lv_obj_t * slider = lv_slider_create(screen_main);
    lv_obj_set_size(slider, 200, 20);
    lv_obj_set_pos(slider, 50, 160);
    
    // 为滑块旋钮应用独立样式
    lv_obj_add_style(slider, &knob_style, LV_PART_KNOB);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    
    // 6. 创建进度条
    lv_obj_t * bar = lv_bar_create(screen_main);
    lv_obj_set_size(bar, 200, 20);
    lv_obj_set_pos(bar, 50, 200);
    lv_bar_set_value(bar, 60, LV_ANIM_ON);
    
    // 7. 创建说明文本
    lv_obj_t * info_label = lv_label_create(screen_main);
    lv_label_set_text(info_label, "完整 LVGL 样式测试说明：\n"
                                  "• 按钮1：基础蓝色样式 + 按压状态\n"
                                  "• 按钮2：红色变体样式\n"
                                  "• 按钮3：橙色本地样式（20px圆角）\n"
                                  "• 滑块：红色圆形旋钮\n"
                                  "• 进度条：默认样式\n"
                                  "• 支持真实的事件处理");
    lv_obj_set_pos(info_label, 50, 250);
    
    printf("✅ 界面创建完成！\n");
}

// 初始化 LVGL
void lvgl_init() {
    printf("正在初始化 LVGL...\n");
    
    // 初始化 LVGL
    lv_init();
    
    // 初始化显示缓冲区
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 800 * 480 / 10);
    
    // 注册显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);
    
    // 注册输入设备驱动
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = input_read_cb;
    lv_indev_drv_register(&indev_drv);
    
    printf("✅ LVGL 初始化完成！\n");
}

int main() {
    printf("=== LVGL 完整样式测试程序 ===\n");
    
    // 初始化随机数种子
    srand(time(NULL));
    
    // 初始化 LVGL
    lvgl_init();
    
    // 设置样式
    setup_styles();
    
    // 创建界面
    create_ui();
    
    printf("程序已启动，按 Ctrl+C 退出\n");
    printf("模拟触摸事件将自动触发...\n\n");
    
    // 主循环
    int counter = 0;
    while(1) {
        lv_task_handler();
        usleep(5000); // 5ms 延时
        
        // 每100次循环模拟一次触摸事件
        counter++;
        if(counter % 100 == 0) {
            printf("模拟触摸事件...\n");
        }
    }
    
    return 0;
} 
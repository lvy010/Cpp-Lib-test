#include "lvgl/lvgl.h"
#include <stdio.h>
#include <unistd.h>

// 声明界面创建函数
void create_ui(void);
void setup_styles(void);

// 全局样式变量
static lv_style_t my_button_style;
static lv_style_t my_button_pressed_style;
static lv_style_t knob_style;
static lv_style_t global_font_style;

int main() {
    // 初始化 LVGL
    lv_init();
    
    // 设置显示缓冲区（模拟环境）
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf1[800 * 480 / 10];
    static lv_color_t buf2[800 * 480 / 10];
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 800 * 480 / 10);
    
    // 注册显示驱动（模拟）
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = NULL; // 模拟环境，不需要实际刷新
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);
    
    // 设置样式
    setup_styles();
    
    // 创建界面
    create_ui();
    
    printf("LVGL 样式测试程序已启动！\n");
    printf("按 Ctrl+C 退出程序\n");
    
    // 主循环
    while(1) {
        lv_task_handler();
        usleep(5000); // 5ms 延时
    }
    
    return 0;
} 
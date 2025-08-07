#include "lvgl.h"
#include <stdio.h>

// 通用事件处理器
static void my_bubbling_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * original_target = lv_event_get_target_obj(e);  // 原始触发控件
    lv_obj_t * current_target = lv_event_get_current_target_obj(e); // 当前处理控件

    if (code == LV_EVENT_CLICKED)
    {
        printf("事件代码: CLICKED\n");
        printf("  原始目标: %s\n", lv_obj_class_get_name(lv_obj_get_class(original_target)));
        printf("  当前处理器: %s\n", lv_obj_class_get_name(lv_obj_get_class(current_target)));
        printf("------------------------\n");
    }
}

void create_bubbling_example()
{
    lv_obj_t * screen_main = lv_obj_create(NULL);
    lv_scr_load(screen_main);

    // 父级面板
    lv_obj_t * parent_panel = lv_obj_create(screen_main);
    lv_obj_set_size(parent_panel, 200, 100);
    lv_obj_align(parent_panel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(parent_panel, my_bubbling_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(parent_panel, LV_OBJ_FLAG_EVENT_BUBBLE); // 启用冒泡

    // 子按钮
    lv_obj_t * child_button = lv_btn_create(parent_panel);
    lv_obj_set_size(child_button, 100, 50);
    lv_obj_align(child_button, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(child_button, my_bubbling_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(child_button, LV_OBJ_FLAG_EVENT_BUBBLE); // 子控件冒泡

    // 标签（需启用点击）
    lv_obj_t * label = lv_label_create(child_button);
    lv_label_set_text(label, "点击我");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(label, LV_OBJ_FLAG_EVENT_BUBBLE); // 标签冒泡
    lv_obj_add_event_cb(label, my_bubbling_event_cb, LV_EVENT_CLICKED, NULL);
}

int main(void)
{
    lv_init();
    // 这里应初始化显示和输入驱动，略去具体实现
    // lv_disp_drv_register(...);
    // lv_indev_drv_register(...);

    create_bubbling_example();

    // 主循环
    while(1) {
        lv_timer_handler();
        // 这里应有延时，实际项目中用lv_tick_inc和系统延时
    }
    return 0;
} 
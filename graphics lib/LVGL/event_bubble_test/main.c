#include <stdio.h>
#include <string.h>

// 对象类型
typedef enum {
    OBJ_TYPE_SCR,
    OBJ_TYPE_PANEL,
    OBJ_TYPE_BUTTON,
    OBJ_TYPE_LABEL
} obj_type_t;

// 对象结构体
typedef struct obj {
    obj_type_t type;
    struct obj * parent;
    const char * name; // 类型名
} obj_t;

// 获取类型名
const char * get_type_name(const obj_t * obj) {
    if(!obj) return "NULL";
    switch(obj->type) {
        case OBJ_TYPE_LABEL: return "label";
        case OBJ_TYPE_BUTTON: return "button";
        case OBJ_TYPE_PANEL: return "obj (parent_panel)";
        case OBJ_TYPE_SCR: return "scr (screen_main)";
        default: return "unknown";
    }
}

// 事件代码
#define LV_EVENT_CLICKED 1

typedef struct {
    int code;
    obj_t * original_target;
    obj_t * current_target;
} event_t;

// 事件回调
void my_bubbling_event_cb(event_t * e) {
    if(e->code == LV_EVENT_CLICKED) {
        printf("事件代码: CLICKED\n");
        printf("  原始目标: %s\n", get_type_name(e->original_target));
        printf("  当前处理器: %s\n", get_type_name(e->current_target));
        printf("------------------------\n");
    }
}

// 事件冒泡
void bubble_event(obj_t * obj, event_t * e) {
    if(!obj) return;
    e->current_target = obj;
    my_bubbling_event_cb(e);
    if(obj->parent) bubble_event(obj->parent, e);
}

int main(void) {
    // 构建对象树
    obj_t scr = {OBJ_TYPE_SCR, NULL, "scr"};
    obj_t panel = {OBJ_TYPE_PANEL, &scr, "parent_panel"};
    obj_t button = {OBJ_TYPE_BUTTON, &panel, "button"};
    obj_t label = {OBJ_TYPE_LABEL, &button, "label"};

    // 模拟点击label
    event_t e = {LV_EVENT_CLICKED, &label, NULL};
    bubble_event(&label, &e);
    return 0;
} 
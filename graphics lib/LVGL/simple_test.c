#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 模拟 LVGL 样式结构
typedef struct {
    int bg_color;
    int text_color;
    int radius;
    int padding;
} lv_style_t;

// 模拟 LVGL 对象结构
typedef struct {
    int x, y, width, height;
    lv_style_t *style;
    char *text;
} lv_obj_t;

// 全局样式变量
static lv_style_t my_button_style;
static lv_style_t my_button_pressed_style;
static lv_style_t red_button_style;
static lv_style_t orange_button_style;

// 样式初始化函数
void lv_style_init(lv_style_t *style) {
    style->bg_color = 0;
    style->text_color = 0;
    style->radius = 0;
    style->padding = 0;
}

// 设置背景颜色
void lv_style_set_bg_color(lv_style_t *style, int color) {
    style->bg_color = color;
}

// 设置圆角
void lv_style_set_radius(lv_style_t *style, int radius) {
    style->radius = radius;
}

// 创建按钮对象
lv_obj_t* create_button(int x, int y, int width, int height, const char *text) {
    lv_obj_t *obj = malloc(sizeof(lv_obj_t));
    obj->x = x;
    obj->y = y;
    obj->width = width;
    obj->height = height;
    obj->text = (char*)text;
    obj->style = NULL;
    return obj;
}

// 应用样式到对象
void apply_style(lv_obj_t *obj, lv_style_t *style) {
    obj->style = style;
}

// 打印对象信息
void print_object(lv_obj_t *obj, const char *name) {
    printf("=== %s ===\n", name);
    printf("位置: (%d, %d)\n", obj->x, obj->y);
    printf("尺寸: %dx%d\n", obj->width, obj->height);
    printf("文本: %s\n", obj->text);
    if (obj->style) {
        printf("背景色: 0x%06X\n", obj->style->bg_color);
        printf("圆角: %dpx\n", obj->style->radius);
        printf("内边距: %dpx\n", obj->style->padding);
    }
    printf("\n");
}

// 样式测试函数
void test_styles() {
    printf("=== LVGL 样式测试程序 ===\n\n");
    
    // 1. 初始化基础按钮样式（蓝色）
    lv_style_init(&my_button_style);
    lv_style_set_bg_color(&my_button_style, 0x007BFF);
    lv_style_set_radius(&my_button_style, 10);
    printf("✅ 基础按钮样式初始化完成（蓝色，10px圆角）\n");
    
    // 2. 初始化按压状态样式（深蓝色）
    lv_style_init(&my_button_pressed_style);
    lv_style_set_bg_color(&my_button_pressed_style, 0x0056b3);
    lv_style_set_radius(&my_button_pressed_style, 10);
    printf("✅ 按压状态样式初始化完成（深蓝色）\n");
    
    // 3. 创建红色变体样式
    lv_style_init(&red_button_style);
    lv_style_set_bg_color(&red_button_style, 0xFF0000);
    lv_style_set_radius(&red_button_style, 10);
    printf("✅ 红色变体样式初始化完成\n");
    
    // 4. 创建橙色本地样式
    lv_style_init(&orange_button_style);
    lv_style_set_bg_color(&orange_button_style, 0xFF8000);
    lv_style_set_radius(&orange_button_style, 20);
    printf("✅ 橙色本地样式初始化完成（20px圆角）\n\n");
    
    // 5. 创建按钮对象
    lv_obj_t *button1 = create_button(50, 80, 120, 50, "按钮1");
    lv_obj_t *button2 = create_button(200, 80, 120, 50, "按钮2");
    lv_obj_t *button3 = create_button(350, 80, 120, 50, "本地样式");
    
    // 6. 应用样式
    apply_style(button1, &my_button_style);
    apply_style(button2, &red_button_style);  // 层叠样式示例
    apply_style(button3, &orange_button_style);
    
    // 7. 打印对象信息
    print_object(button1, "按钮1 - 基础蓝色样式");
    print_object(button2, "按钮2 - 红色变体样式");
    print_object(button3, "按钮3 - 橙色本地样式");
    
    // 8. 演示状态变化
    printf("=== 状态变化演示 ===\n");
    printf("按钮1 默认状态 -> 按压状态\n");
    apply_style(button1, &my_button_pressed_style);
    print_object(button1, "按钮1 - 按压状态");
    
    // 9. 清理内存
    free(button1);
    free(button2);
    free(button3);
    
    printf("✅ 样式测试完成！\n");
    printf("演示了以下功能：\n");
    printf("• 基础样式设置\n");
    printf("• 状态样式变化\n");
    printf("• 层叠样式应用\n");
    printf("• 本地样式设置\n");
    printf("• 圆角和颜色配置\n");
}

int main() {
    printf("正在启动 LVGL 样式测试程序...\n");
    sleep(1);
    
    test_styles();
    
    printf("\n程序运行完成！\n");
    return 0;
} 
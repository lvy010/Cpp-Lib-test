#ifndef LV_INDEV_H
#define LV_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// 前向声明
struct _lv_display_t;
struct _lv_timer_t;
struct _lv_group_t;
struct _lv_obj_t;

// 输入设备类型枚举
typedef enum {
    LV_INDEV_TYPE_NONE = 0,
    LV_INDEV_TYPE_POINTER,    // 触摸屏、鼠标等
    LV_INDEV_TYPE_KEYPAD,     // 键盘
    LV_INDEV_TYPE_ENCODER,    // 编码器
    LV_INDEV_TYPE_BUTTON,     // 按钮
} lv_indev_type_t;

// 输入设备状态枚举
typedef enum {
    LV_INDEV_STATE_RELEASED = 0,
    LV_INDEV_STATE_PRESSED,
} lv_indev_state_t;

// 输入设备数据结构
typedef struct {
    lv_point_t point;         // 坐标点
    uint32_t key;             // 按键值
    lv_indev_state_t state;   // 状态
    bool continue_reading;     // 是否继续读取
} lv_indev_data_t;

// 输入设备读取回调函数类型
typedef void (*lv_indev_read_cb_t)(struct _lv_indev_t * indev, lv_indev_data_t * data);

// 指针输入设备特定数据
typedef struct {
    lv_point_t act_point;     // 当前活动点
    lv_point_t last_point;    // 上一个点
    lv_obj_t * act_obj;       // 当前活动对象
    lv_obj_t * last_obj;      // 上一个对象
    lv_point_t scroll_sum;    // 滚动累计
    lv_point_t scroll_throw_vect;
    uint8_t scroll_dir;
    bool scroll_inv;
    uint8_t gesture_dir;
    uint8_t gesture_sum;
    uint8_t gesture_last_dir;
    uint8_t gesture_sent;
} lv_indev_pointer_t;

// 键盘输入设备特定数据
typedef struct {
    uint32_t last_key;
    uint16_t last_state;
} lv_indev_keypad_t;

// 编码器输入设备特定数据
typedef struct {
    int16_t diff;
    lv_obj_t * act_obj;
    lv_obj_t * last_obj;
} lv_indev_encoder_t;

// 按钮输入设备特定数据
typedef struct {
    uint32_t last_key;
    uint16_t last_state;
} lv_indev_button_t;

// 输入设备主结构体
typedef struct _lv_indev_t {
    lv_indev_type_t type;            /**< 输入设备类型（POINTER, KEYPAD, ENCODER, BUTTON） */
    lv_indev_read_cb_t read_cb;      /**< 输入设备数据读取函数 */
    lv_indev_state_t state;          /**< 当前状态（PRESSED或RELEASED） */
    struct _lv_display_t * disp;     /**< 关联的显示设备 */
    lv_timer_t * read_timer;         /**< 周期性调用read_cb的定时器 */
    lv_group_t * group;              /**< 针对KEYPAD/ENCODER：交互的控件组 */
    
    // 设备特定数据
    union {
        lv_indev_pointer_t pointer;
        lv_indev_keypad_t keypad;
        lv_indev_encoder_t encoder;
        lv_indev_button_t button;
    };
    
    // 手势和长按相关参数
    uint32_t long_time;
    uint32_t long_repeat_time;
    uint32_t gesture_time;
    uint32_t gesture_repeat_time;
    uint32_t scroll_time;
    uint32_t scroll_repeat_time;
    
    // 内部状态
    bool long_pr_sent;
    bool gesture_sent;
    bool scroll_sent;
    bool gesture_recognized;
} lv_indev_t;

// 函数声明
lv_indev_t * lv_indev_create(void);
void lv_indev_set_read_cb(lv_indev_t * indev, lv_indev_read_cb_t read_cb);
void lv_indev_read(lv_indev_t * indev);
void lv_indev_set_type(lv_indev_t * indev, lv_indev_type_t type);
void lv_indev_set_display(lv_indev_t * indev, struct _lv_display_t * disp);

// 内部处理函数（通常不对外暴露）
static void indev_pointer_proc(lv_indev_t * i, lv_indev_data_t * data);
static void indev_proc_press(lv_indev_t * indev);
static void indev_proc_release(lv_indev_t * indev);

#ifdef __cplusplus
}
#endif

#endif // LV_INDEV_H 
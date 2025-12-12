# LVGL 输入设备模块

这个文件夹包含了LVGL输入设备系统的核心代码，展示了LVGL如何处理各种输入设备（触摸屏、鼠标、键盘、编码器、按钮等）。

## 文件说明

### lv_indev.h

- 输入设备系统的头文件
- 定义了完整的`lv_indev_t`结构体
- 包含各种输入设备类型的数据结构
- 声明了主要的API函数

### lv_indev.c

- 输入设备系统的实现文件
- 包含输入设备的创建、配置和处理逻辑
- 实现了指针输入设备的处理流程
- 展示了事件驱动的输入处理机制

## 核心概念

### 输入设备类型 (lv_indev_type_t)

- `LV_INDEV_TYPE_POINTER`: 触摸屏、鼠标等指针设备
- `LV_INDEV_TYPE_KEYPAD`: 键盘输入
- `LV_INDEV_TYPE_ENCODER`: 编码器输入
- `LV_INDEV_TYPE_BUTTON`: 按钮输入

### 输入设备状态 (lv_indev_state_t)

- `LV_INDEV_STATE_RELEASED`: 释放状态
- `LV_INDEV_STATE_PRESSED`: 按压状态

### 主要数据结构

#### lv_indev_t

输入设备的主结构体，包含：

- 设备类型和状态
- 读取回调函数
- 关联的显示设备
- 定时器用于周期性读取
- 设备特定的数据（指针、键盘、编码器、按钮）

#### lv_indev_data_t

输入数据容器，包含：

- 坐标点信息
- 按键值
- 状态信息
- 是否继续读取标志

## 使用示例

```c
// 创建输入设备
lv_indev_t * indev = lv_indev_create();

// 设置设备类型
lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

// 设置读取回调函数
lv_indev_set_read_cb(indev, my_read_callback);

// 读取回调函数示例
void my_read_callback(lv_indev_t * indev, lv_indev_data_t * data)
{
    // 读取硬件输入数据
    data->point.x = get_touch_x();
    data->point.y = get_touch_y();
    data->state = get_touch_state();
    data->continue_reading = true;
}
```

## 处理流程

1. **创建阶段**: `lv_indev_create()` 分配内存并初始化默认值
2. **配置阶段**: 设置设备类型、读取回调、显示设备等
3. **运行阶段**: 定时器周期性调用 `lv_indev_read()`
4. **数据处理**: 根据设备类型调用相应的处理函数
5. **事件生成**: 处理按压、释放、长按、手势等事件

## 注意事项

- 这是简化版的实现，实际的LVGL会有更复杂的逻辑
- 包含了一些假设的函数（如定时器、链表操作等）
- 主要用于学习和理解LVGL输入设备的工作原理
- 实际使用时需要集成到完整的LVGL框架中

## 扩展功能

可以基于这个基础框架添加：

- 键盘输入处理
- 编码器输入处理
- 按钮输入处理
- 更复杂的手势识别
- 多点触控支持
- 输入设备热插拔支持

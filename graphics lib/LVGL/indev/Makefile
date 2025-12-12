# LVGL 输入设备模块 Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -I.
LIBS = 

# 源文件和目标文件
SOURCES = lv_indev.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = lv_indev_test

# 默认目标
all: $(TARGET)

# 编译目标文件
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 安装（如果需要）
install:
	# 这里可以添加安装逻辑
	@echo "安装完成"

# 帮助信息
help:
	@echo "可用的目标:"
	@echo "  all     - 编译所有文件"
	@echo "  clean   - 清理编译文件"
	@echo "  install - 安装（如果需要）"
	@echo "  help    - 显示此帮助信息"

.PHONY: all clean install help 
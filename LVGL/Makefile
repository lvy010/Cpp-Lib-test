CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -llvgl

# 目标文件
TARGET = lvgl_style_test
SOURCES = main.c styles.c
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 运行
run: $(TARGET)
	./$(TARGET)

# 安装依赖（Ubuntu/Debian）
install-deps:
	sudo apt update
	sudo apt install -y liblvgl-dev build-essential

.PHONY: all clean run install-deps 
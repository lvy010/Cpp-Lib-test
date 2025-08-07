CC = gcc
CFLAGS = -Wall -Wextra -g -I../.. -I../lvgl -I../lvgl/src
LDFLAGS = 

# 递归查找LVGL所有C文件
LVGL_SRC := $(shell find ../lvgl/src -name '*.c')
SRC = main.c
OBJ = $(SRC:.c=.o)
LVGL_OBJ = $(LVGL_SRC:.c=.o)
TARGET = event_bubble_test

all: $(TARGET)

$(TARGET): $(OBJ) $(LVGL_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LVGL_OBJ) $(TARGET)

.PHONY: all clean 
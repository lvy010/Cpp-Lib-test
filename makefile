# Makefile for compiling test_eventloop with muduo

# --- Configuration ---
# 设置 muduo 的安装目录。
# 这个目录是在 muduo 构建目录中执行 `make install` 后创建的。
MUDUO_INSTALL_DIR := ./build/release-install-cpp11

# --- Compiler and Flags ---
CXX := g++
# CXXFLAGS: -std=c++11 是 muduo 的要求。-g 用于调试。-Wall 显示所有警告。
# -I 标志告诉编译器在哪里寻找头文件。我们指向 muduo 安装目录下的 'include'。
CXXFLAGS := -std=c++11 -g -Wall -I$(MUDUO_INSTALL_DIR)/include

# --- Linker Flags and Libraries ---
# -L 标志告诉链接器在哪里寻找库文件。
LDFLAGS := -L$(MUDUO_INSTALL_DIR)/lib
# -l 标志指定要链接哪些库。
LDLIBS := -lmuduo_net -lmuduo_base -lpthread -lboost_system

# --- Targets ---
TARGETS := test_eventloop test_thread_eventloop test_simple_thread test_timerfd_channel test_buffer_test
SRC := test/test_eventloop.cpp

.PHONY: all clean run check_muduo

# 默认目标
all: $(TARGETS)

# 各个目标的编译规则

test_eventloop: test/test_eventloop.cpp | check_muduo
	@echo "==> 编译并链接 test_eventloop..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 test_eventloop"

test_thread_eventloop: test/test_thread_eventloop.cpp | check_muduo
	@echo "==> 编译并链接 test_thread_eventloop..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 test_thread_eventloop"

test_simple_thread: test/test_simple_thread.cpp | check_muduo
	@echo "==> 编译并链接 test_simple_thread..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 test_simple_thread"

test_timerfd_channel: test/timerfd_channel.cpp | check_muduo
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

test_buffer_test: test/buffer_test.cpp | check_muduo
	@echo "==> 编译并链接 test_buffer_test..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 test_buffer_test"

# 检查 muduo 库是否已构建并安装
check_muduo:
	@if [ ! -d "$(MUDUO_INSTALL_DIR)/lib" ] || [ ! -f "$(MUDUO_INSTALL_DIR)/lib/libmuduo_net.a" ]; then \
		echo "错误：在 $(MUDUO_INSTALL_DIR)/lib 中找不到 muduo 库"; \
		echo "提示：您是否已在 muduo 构建目录（例如 build/release-cpp11）中运行 'make install'？"; \
		exit 1; \
	fi

run: test_eventloop
	@echo "==> 运行 test_eventloop..."
	@echo "---"
	LD_LIBRARY_PATH=$(MUDUO_INSTALL_DIR)/lib:$$LD_LIBRARY_PATH ./test_eventloop

clean:
	@echo "==> 清理..."
	rm -f test_eventloop test_thread_eventloop test_simple_thread test_timerfd_channel test_buffer_test
	rm -f test_eventloop test_thread_eventloop test_simple_thread test_timerfd_channel
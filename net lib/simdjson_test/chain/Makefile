# Makefile for simdjson tests

# --- Configuration ---
CXX := g++
CXXFLAGS := -std=c++17 -g -Wall -O2
LDFLAGS := 
LDLIBS := -lsimdjson

# --- Targets ---
TARGETS := simdjson_chain_test simdjson_chain_comprehensive_test

.PHONY: all clean run run_all

# 默认目标
all: $(TARGETS)

# 编译规则
simdjson_chain_test: simdjson_chain_test.cpp
	@echo "==> 编译 simdjson_chain_test..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 simdjson_chain_test"

simdjson_chain_comprehensive_test: simdjson_chain_comprehensive_test.cpp
	@echo "==> 编译 simdjson_chain_comprehensive_test..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@echo "==> 成功构建 simdjson_chain_comprehensive_test"

# 运行测试
run: simdjson_chain_test
	@echo "==> 运行基础链式调用测试..."
	@echo "---"
	./simdjson_chain_test

run_comprehensive: simdjson_chain_comprehensive_test
	@echo "==> 运行综合链式调用测试..."
	@echo "---"
	./simdjson_chain_comprehensive_test

run_all: $(TARGETS)
	@echo "==> 运行所有链式调用测试..."
	@echo "---"
	@echo "1. 基础链式调用测试:"
	./simdjson_chain_test
	@echo ""
	@echo "2. 综合链式调用测试:"
	./simdjson_chain_comprehensive_test

# 清理
clean:
	@echo "==> 清理 simdjson 测试文件..."
	rm -f $(TARGETS)

# 帮助信息
help:
	@echo "可用的目标:"
	@echo "  all              - 编译所有测试"
	@echo "  run              - 运行基础链式调用测试"
	@echo "  run_comprehensive - 运行综合链式调用测试"
	@echo "  run_all          - 运行所有测试"
	@echo "  clean            - 清理编译文件"
	@echo "  help             - 显示此帮助信息" 
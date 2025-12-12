#!/bin/bash

echo "=========================================="
echo "simdjson 链式调用测试演示"
echo "=========================================="

# 检查 simdjson 库是否安装
echo "检查 simdjson 库..."
if pkg-config --exists simdjson; then
    echo "✓ simdjson 库已安装"
else
    echo "✗ simdjson 库未安装，请先安装："
    echo "  sudo apt-get install libsimdjson-dev"
    exit 1
fi

# 编译测试
echo ""
echo "编译测试程序..."
make clean
make all

if [ $? -eq 0 ]; then
    echo "✓ 编译成功"
else
    echo "✗ 编译失败"
    exit 1
fi

# 运行基础测试
echo ""
echo "运行基础链式调用测试..."
echo "------------------------------------------"
./simdjson_chain_test

# 运行综合测试
echo ""
echo "运行综合链式调用测试..."
echo "------------------------------------------"
./simdjson_chain_comprehensive_test

echo ""
echo "=========================================="
echo "测试完成！"
echo "==========================================" 
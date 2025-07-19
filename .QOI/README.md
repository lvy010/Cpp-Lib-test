# QOI基准测试

## 参考资料

- [QOI官方仓库](https://github.com/phoboslab/qoi)
- [QOI格式规范](https://qoiformat.org/)
- [libpng文档](http://www.libpng.org/pub/png/libpng.html)


## 简介

QOI (Quite OK Image) 是一种快速、无损的图像压缩格式。本项目提供了一个基准测试工具，用于评估QOI格式的编码和解码性能。

### QOI格式特点
- **无损压缩**: 完全保留原始图像质量
- **快速编码/解码**: 比传统格式如PNG更快
- **简单实现**: 单头文件实现，易于集成
- **无外部依赖**: 不依赖复杂的压缩库

## 项目结构

```
.QOI/
├── qoi/                    # QOI源码目录
│   ├── qoi.h              # QOI格式实现
│   ├── qoi_simple_bench.c # 基准测试工具
│   ├── qoibench.c         # 原始基准测试工具
│   ├── qoiconv.c          # 格式转换工具
│   ├── Makefile           # 编译配置
│   └── README.md          # QOI项目说明
├── test/                   # 测试图像目录
│   └── test.png           # 测试用PNG图像
└── README.md              # 本文件
```

## 环境要求

### 系统依赖
- Linux系统 (推荐Ubuntu 18.04+)
- GCC编译器 (支持C99标准)
- libpng开发库

### 安装依赖

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential libpng-dev

# CentOS/RHEL
sudo yum install gcc libpng-devel

# 或者使用dnf (较新版本)
sudo dnf install gcc libpng-devel
```

## 编译安装

### 1. 编译基准测试工具

```bash
cd .QOI/qoi
gcc -std=c99 -O3 qoi_simple_bench.c -o qoi_simple_bench -lpng -lrt
```

### 2. 验证编译结果

```bash
ls -la qoi_simple_bench
# 应该显示可执行文件
```

## 使用方法

### 基本语法

```bash
./qoi_simple_bench <运行次数> <图像目录路径>
```

### 参数说明
- `<运行次数>`: 每个测试重复执行的次数 (推荐5-10次)
- `<图像目录路径>`: 包含PNG图像的目录路径

### 使用示例

```bash
# 对test目录中的图像执行5轮测试
./qoi_simple_bench 5 ../test/

# 对my_images目录中的图像执行10轮测试
./qoi_simple_bench 10 ../../my_images/
```

## 测试结果示例

### 测试图像信息
- **图像**: test.png (初音未来角色图)
- **尺寸**: 1031×565 像素
- **格式**: RGBA (4通道)
- **原始大小**: 2,330,060 字节 (约2.33MB)

### QOI编码性能 (5轮测试)

[ret](.QOI/test/ret.png)

## 性能分析

### 优势
1. **高压缩率**: 61.1%的压缩率，将2.33MB压缩到906KB
2. **快速编码**: 平均5.57ms，适合实时应用
3. **极快解码**: 平均4.27ms，解码速度136.5 MP/s
4. **性能稳定**: 5轮测试结果一致性很好

### 适用场景
- **游戏开发**: 快速加载纹理图像
- **Web应用**: 需要快速解码的图像
- **实时系统**: 对解码速度要求高的场景
- **移动应用**: 需要平衡压缩率和速度的场景

## 技术细节

### 测试环境
- **操作系统**: Linux 6.8.0-62-generic
- **编译器**: GCC (C99标准)
- **优化级别**: -O3
- **计时精度**: 纳秒级 (clock_gettime)

### 测试方法
1. **图像加载**: 使用libpng读取PNG文件
2. **编码测试**: 测量QOI编码耗时和输出大小
3. **解码测试**: 测量QOI解码耗时
4. **多次运行**: 每个测试重复执行指定次数
5. **统计分析**: 计算平均值和性能指标

### 性能指标说明
- **MP/s**: 百万像素/秒，表示每秒处理的像素数量
- **压缩率**: (1 - 压缩后大小/原始大小) × 100%
- **编码/解码时间**: 毫秒级精度

## 故障排除

### 常见问题

1. **编译错误**: `fatal error: png.h: No such file or directory`
   ```bash
   # 解决方案：安装libpng开发库
   sudo apt-get install libpng-dev
   ```

2. **链接错误**: `undefined reference to 'clock_gettime'`
   ```bash
   # 解决方案：添加lrt库
   gcc -std=c99 -O3 qoi_simple_bench.c -o qoi_simple_bench -lpng -lrt
   ```

3. **运行时错误**: `无法读取文件`
   ```bash
   # 检查文件路径和权限
   ls -la <图像文件路径>
   file <图像文件路径>  # 确认是有效的PNG文件
   ```

### 调试模式
```bash
# 使用调试信息编译
gcc -std=c99 -g -O0 qoi_simple_bench.c -o qoi_simple_bench_debug -lpng -lrt

# 使用GDB调试
gdb ./qoi_simple_bench_debug
```

## 扩展功能

### 自定义测试
可以修改`qoi_simple_bench.c`来添加更多测试功能：
- 不同图像格式的对比测试
- 内存使用量统计
- 更详细的性能分析

### 批量测试
```bash
# 创建测试脚本
#!/bin/bash
for i in {1..10}; do
    echo "=== 测试轮次 $i ==="
    ./qoi_simple_bench 5 ../test/
    echo ""
done
```

## 许可证

本项目基于MIT许可证开源。QOI格式本身也采用MIT许可证。

## 贡献

欢迎提交Issue和Pull Request来改进这个基准测试工具！

---

*最后更新: 2024年7月19日* 
# SimdJSON 测试集合

这是一个全面的 SimdJSON 库测试项目，展示了如何使用 SimdJSON 进行高性能 JSON 解析的各种场景和技术。SimdJSON 是一个极快的 JSON 解析库，利用 SIMD 指令实现了卓越的性能。

## 📁 项目结构

```
simdjson_test/
├── README.md                    # 本文档
├── test_simdjson.cpp           # 基础 JSON 解析入门示例
├── chain/                      # 链式调用测试
│   ├── README.md              # 链式调用详细说明
│   ├── Makefile              # 编译配置
│   ├── demo.sh               # 演示脚本
│   ├── simdjson_chain_test.cpp
│   └── simdjson_chain_comprehensive_test.cpp
├── error/                      # 错误处理测试
│   ├── simdjson_error_test.cpp
│   ├── simdjson_exception_test.cpp
│   ├── simdjson_fatal_error_test.cpp
│   └── simdjson_simple_error_test.cpp
└── stream/                     # 流式处理测试
    ├── Makefile
    └── simdjson_ndjson_stream_test.cpp
```

## 🚀 功能特性

### 核心功能测试
- **基础解析**: JSON 文档的基本解析和类型检查
- **链式访问**: 使用 ondemand API 进行高效的嵌套数据访问
- **错误处理**: 完善的错误处理机制和异常管理
- **流式处理**: NDJSON (换行分隔的 JSON) 文档流处理

### 性能优势
- 🏃‍♂️ **极高性能**: 利用 SIMD 指令加速解析
- 📈 **内存高效**: 延迟解析和零拷贝设计
- 🔄 **流式处理**: 支持大型 JSON 文件的流式解析
- 🛡️ **类型安全**: 编译时类型检查

## 📋 前置要求

### 系统依赖
- C++17 或更高版本的编译器
- CMake 3.15+ (可选，用于从源码构建)
- SimdJSON 库

### 安装 SimdJSON

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libsimdjson-dev
```

#### 从源码安装
```bash
git clone https://github.com/simdjson/simdjson.git
cd simdjson
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

#### macOS (Homebrew)
```bash
brew install simdjson
```

## 🔧 编译与运行

### 快速开始
```bash
# 克隆项目（如果需要）
cd simdjson_test

# 编译基础示例
g++ -O3 -std=c++17 test_simdjson.cpp -lsimdjson -o test_simdjson

# 运行基础示例
./test_simdjson
```

### 分模块编译

#### 链式调用测试
```bash
cd chain/
make all          # 编译所有链式调用测试
make run         # 运行基础链式调用测试
make run_comprehensive  # 运行综合测试
```

#### 错误处理测试
```bash
cd error/
# 编译各种错误处理测试
g++ -O3 -std=c++17 simdjson_error_test.cpp -lsimdjson -o error_test
g++ -O3 -std=c++17 simdjson_exception_test.cpp -lsimdjson -o exception_test
```

#### 流式处理测试
```bash
cd stream/
make            # 编译流式处理测试
make run        # 运行 NDJSON 流处理测试
```

## 📚 测试模块详解

### 1. 基础解析 (`test_simdjson.cpp`)
演示 SimdJSON 的基本使用方法：
```cpp
simdjson::ondemand::parser parser;
simdjson::padded_string json_data = R"({"message": "hello world", "status": true})"_padded;
simdjson::ondemand::document doc = parser.iterate(json_data);
```

**学习要点**:
- 解析器初始化
- 文档迭代
- 类型检查
- 基础错误处理

### 2. 链式调用 (`chain/`)
展示 SimdJSON 强大的链式访问能力：
```cpp
std::string_view city_name;
error = doc["user"]["address"]["city"].get_string().get(city_name);
```

**特性**:
- 一行代码访问深层嵌套数据
- 延迟解析优化性能
- 统一的错误处理
- 类型安全保障

### 3. 错误处理 (`error/`)
全面的错误处理策略：
- **简单错误处理**: 基础的错误码检查
- **异常处理**: 使用异常机制处理错误
- **致命错误**: 处理严重的解析错误
- **综合错误测试**: 多种错误场景的测试

### 4. 流式处理 (`stream/`)
处理大型和多文档 JSON 数据：
```cpp
auto stream = parser.iterate_many(ndjson_data);
for (auto doc : stream) {
    // 处理每个文档
}
```

**应用场景**:
- 日志文件处理
- 大数据集解析
- 实时数据流
- NDJSON 格式支持

## 🎯 使用场景

- **Web API 开发**: 快速解析 JSON 响应
- **数据处理**: 大规模 JSON 数据分析
- **日志分析**: 结构化日志文件处理
- **配置管理**: 复杂配置文件解析
- **实时系统**: 低延迟 JSON 处理

## 🔍 性能对比

SimdJSON 相比传统 JSON 库的性能优势：
- 比 RapidJSON 快 2-3 倍
- 比 nlohmann/json 快 5-10 倍
- 内存使用量更低
- CPU 缓存友好

## 📝 最佳实践

1. **使用 `padded_string`**: 确保 JSON 数据正确填充
2. **检查错误**: 始终检查解析和访问操作的错误码
3. **保持数据有效**: 确保原始 JSON 数据在使用期间保持有效
4. **使用 `string_view`**: 避免不必要的字符串拷贝
5. **合理使用链式调用**: 在性能敏感场景下使用链式访问

## 🐛 常见问题

### Q: 为什么需要 `_padded` 后缀？
A: SimdJSON 需要在 JSON 字符串末尾添加填充字节以安全地使用 SIMD 指令。

### Q: 解析后的数据何时失效？
A: 当原始的 `padded_string` 或 `parser` 对象被销毁时，所有派生的视图都会失效。

### Q: 如何处理大型 JSON 文件？
A: 使用流式 API (`iterate_many`) 逐个处理文档，避免将整个文件加载到内存。

## 🤝 贡献指南

欢迎提交问题和改进建议：
1. 报告 bug 或性能问题
2. 提交新的测试用例
3. 完善文档和示例
4. 优化现有代码

## 📖 参考资源

- [SimdJSON 官方文档](https://github.com/simdjson/simdjson)
- [SimdJSON API 参考](https://simdjson.org/api/0.7.0/)
- [性能基准测试](https://github.com/simdjson/simdjson#performance-results)
- [C++ JSON 库比较](https://github.com/miloyip/nativejson-benchmark)

## 📄 许可证

本测试项目遵循 MIT 许可证。SimdJSON 库本身采用 Apache 2.0 许可证。

---

**注意**: 这些测试示例仅用于学习和演示目的。在生产环境中使用时，请根据具体需求调整错误处理和性能优化策略。

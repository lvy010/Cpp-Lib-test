# simdjson 链式调用测试

这个目录包含了 simdjson 库的链式调用测试代码，演示了如何使用 simdjson 的 ondemand API 进行链式访问 JSON 数据。

## 文件说明

- `simdjson_chain_test.cpp` - 基础的链式调用测试，包含您提供的示例代码
- `simdjson_chain_comprehensive_test.cpp` - 综合测试，包含多种场景的链式调用示例
- `Makefile` - 编译配置文件

## 编译和运行

### 前提条件

确保系统已安装 simdjson 库：
```bash
# Ubuntu/Debian
sudo apt-get install libsimdjson-dev

# 或者从源码编译安装
git clone https://github.com/simdjson/simdjson.git
cd simdjson
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

### 编译测试

```bash
cd simdjson_test
make all
```

### 运行测试

```bash
# 运行基础链式调用测试
make run

# 运行综合链式调用测试
make run_comprehensive

# 运行所有测试
make run_all
```

## 测试内容

### 基础链式调用测试 (`simdjson_chain_test.cpp`)

演示了基本的嵌套 JSON 结构访问：
```cpp
error = doc["user"]["address"]["city"].get_string().get(city_name);
```

### 综合链式调用测试 (`simdjson_chain_comprehensive_test.cpp`)

包含以下测试场景：

1. **成功的链式调用** - 访问各种类型的嵌套数据（字符串、数字、数组）
2. **缺失字段处理** - 处理 JSON 中不存在的字段
3. **类型不匹配处理** - 处理字段类型与期望类型不符的情况
4. **深层嵌套访问** - 访问多层嵌套的 JSON 结构

## 错误处理

测试代码演示了如何正确处理链式调用中的错误：

- `simdjson::NO_SUCH_FIELD` - 字段不存在
- `simdjson::INCORRECT_TYPE` - 字段类型不匹配
- `simdjson::SUCCESS` - 操作成功

## 链式调用的优势

1. **简洁性** - 一行代码访问深层嵌套数据
2. **性能** - 延迟解析，只解析需要的数据
3. **错误处理** - 在链式末端统一处理错误
4. **类型安全** - 编译时类型检查

## 注意事项

- 链式调用中的每个步骤都可能失败，但错误只在最终 `.get()` 调用时检查
- 使用 `std::string_view` 避免不必要的字符串拷贝
- 确保 JSON 数据在解析过程中保持有效 
# Drogon_2

本文档记录了完整的代码生成、编译和运行过程的实际输出。

## ✅  1：创建项目

### 命令

```bash
cd /root/drogon_net
drogon_ctl create project MyFirstDrogonApp
```

### 输出

```
create a project named MyFirstDrogonApp
```

---

## ✅ 步骤 2：创建控制器

### 命令

```bash
cd MyFirstDrogonApp
drogon_ctl create controller MyHomeController
```

### 输出

```
Create a http simple controller: MyHomeController
```

---

## ✅ 步骤 3：生成的项目结构

```
MyFirstDrogonApp/
├── build/                      # 构建目录
├── controllers/                # 控制器目录
│   ├── MyHomeController.h     # 控制器头文件
│   └── MyHomeController.cc    # 控制器实现
├── filters/                    # 过滤器目录
├── models/                     # 数据模型目录
├── plugins/                    # 插件目录
├── views/                      # 视图目录
├── test/                       # 测试目录
├── CMakeLists.txt             # CMake 配置
├── config.json                # JSON 配置文件
├── config.yaml                # YAML 配置文件
└── main.cc                    # 主程序入口
```

---

## ✅ 步骤 4：编写代码

### 文件 1: `controllers/MyHomeController.h`

```cpp
// MyHomeController.h
#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class MyHomeController : public drogon::HttpController<MyHomeController>
{
  public:
    METHOD_LIST_BEGIN
    // 添加方法列表
    METHOD_ADD(MyHomeController::hello, "/hello", Get);
    METHOD_ADD(MyHomeController::welcome, "/welcome/{name}", Get);
    METHOD_LIST_END

    // 处理函数声明
    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) const;
  
    void welcome(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &name) const;
};
```

### 文件 2: `controllers/MyHomeController.cc`

```cpp
// MyHomeController.cc
#include "MyHomeController.h"
#include <drogon/HttpResponse.h>

void MyHomeController::hello(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) const
{
    // 创建一个新的 HTTP 响应对象
    auto resp = drogon::HttpResponse::newHttpResponse();
  
    // 设置响应内容
    resp->setBody("Hello from Drogon Controller!");
  
    // 设置内容类型
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
  
    // 通过回调函数发送响应
    callback(resp);
}

void MyHomeController::welcome(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               const std::string &name) const
{
    // 创建响应
    auto resp = drogon::HttpResponse::newHttpResponse();
  
    // 使用路径参数构造响应内容
    resp->setBody("Welcome, " + name + "!");
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
  
    // 发送响应
    callback(resp);
}
```

### 文件 3: `main.cc`

```cpp
// main.cc
#include <drogon/drogon.h>
#include <iostream>

int main()
{
    // 设置日志级别
    drogon::app().setLogLevel(trantor::Logger::kInfo);
  
    // 配置监听地址和端口
    drogon::app().addListener("127.0.0.1", 8848);
  
    // 设置线程数
    drogon::app().setThreadNum(4);
  
    // 输出服务器信息
    std::cout << "==========================================" << std::endl;
    std::cout << "Server running on http://127.0.0.1:8848" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Test URLs:" << std::endl;
    std::cout << "  - http://127.0.0.1:8848/MyHomeController/hello" << std::endl;
    std::cout << "  - http://127.0.0.1:8848/MyHomeController/welcome/Alice" << std::endl;
    std::cout << "==========================================" << std::endl;
  
    // 启动 Drogon 应用程序
    drogon::app().run();
  
    return 0;
}
```

---

## ✅ 步骤 5：编译项目

### 命令

```bash
cd build
cmake ..
make -j$(nproc)
```

### 输出（简化）

```
-- The CXX compiler identification is GNU 13.3.0
-- Looking for C++ include any - found
-- Looking for C++ include string_view - found
-- Found Jsoncpp: /usr/include/jsoncpp
-- jsoncpp version:1.9.5
-- Found OpenSSL: /usr/lib/x86_64-linux-gnu/libcrypto.so (found version "3.0.13")
-- Found Threads: TRUE
-- Found UUID: /usr/lib/x86_64-linux-gnu/libuuid.so
-- Found ZLIB: /usr/lib/x86_64-linux-gnu/libz.so (found version "1.3")
-- use c++17
-- Configuring done (3.3s)
-- Generating done (0.0s)
-- Build files have been written to: /root/drogon_net/MyFirstDrogonApp/build

[ 40%] Building CXX object CMakeFiles/MyFirstDrogonApp.dir/main.cc.o
[ 60%] Building CXX object CMakeFiles/MyFirstDrogonApp.dir/controllers/MyHomeController.cc.o
[ 80%] Linking CXX executable MyFirstDrogonApp
[ 80%] Built target MyFirstDrogonApp
[100%] Linking CXX executable MyFirstDrogonApp_test
[100%] Built target MyFirstDrogonApp_test
```

✅ **编译成功！**

---

## ✅ 步骤 6：运行应用程序

### 命令

```bash
./MyFirstDrogonApp
```

### 输出

```
==========================================
Server running on http://127.0.0.1:8848
==========================================
Test URLs:
  - http://127.0.0.1:8848/MyHomeController/hello
  - http://127.0.0.1:8848/MyHomeController/welcome/Alice
==========================================
```

✅ **服务器启动成功！**

---

## ✅ 步骤 7：测试 API 端点

### 测试 1: `/hello` 端点

#### 命令

```bash
curl http://127.0.0.1:8848/MyHomeController/hello
```

#### 输出

```
Hello from Drogon Controller!
```

✅ **测试通过！**

---

### 测试 2: `/welcome/{name}` 端点

#### 测试 Alice

```bash
curl http://127.0.0.1:8848/MyHomeController/welcome/Alice
```

**输出:**

```
Welcome, Alice!
```

#### 测试 Bob

```bash
curl http://127.0.0.1:8848/MyHomeController/welcome/Bob
```

**输出:**

```
Welcome, Bob!
```

#### 测试 Developer

```bash
curl http://127.0.0.1:8848/MyHomeController/welcome/Developer
```

**输出:**

```
Welcome, Developer!
```

✅ **所有测试通过！**

---

## ✅ 步骤 8：使用浏览器测试

### 在浏览器中访问

#### URL 1

```
http://127.0.0.1:8848/MyHomeController/hello
```

**浏览器显示:**

```
Hello from Drogon Controller!
```

#### URL 2

```
http://127.0.0.1:8848/MyHomeController/welcome/YourName
```

**浏览器显示:**

```
Welcome, YourName!
```

---

## 📊 关键技术要点总结

### 1. 控制器自动注册机制

- 通过继承 `HttpController<T>` 实现自动注册
- 使用 `METHOD_LIST_BEGIN/END` 宏定义路由列表
- `METHOD_ADD` 宏将 URL 映射到处理函数

### 2. 路由配置

```cpp
METHOD_ADD(MyHomeController::hello, "/hello", Get);
METHOD_ADD(MyHomeController::welcome, "/welcome/{name}", Get);
```

- 第一个参数：处理函数
- 第二个参数：URL 路径（支持路径参数 `{name}`）
- 第三个参数：HTTP 方法

### 3. 异步回调模式

```cpp
void hello(const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("Hello!");
    callback(resp);  // 通过回调发送响应
}
```

### 4. 路径参数捕获

```cpp
void welcome(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback,
             const std::string &name) const  // name 参数自动捕获
{
    resp->setBody("Welcome, " + name + "!");
    callback(resp);
}
```

---

## 🎉 成功指标

✅ Drogon 框架成功编译安装
✅ 项目成功创建
✅ 控制器成功生成
✅ 代码成功编译
✅ 服务器成功启动
✅ 所有 API 端点测试通过
✅ 路径参数功能正常
✅ 异步响应机制工作正常

---

## 📝 环境信息

- **操作系统**: Linux (Ubuntu Noble)
- **编译器**: GNU C++ 13.3.0
- **C++ 标准**: C++17
- **Drogon 版本**: 1.9.11
- **监听地址**: 127.0.0.1:8848
- **线程数**: 4

---

## 🚀 下一步学习

完成了第 2 章的实战后，我们已经掌握了：

1. 如何使用 `drogon_ctl` 创建项目和控制器
2. 控制器的基本结构和路由配置
3. 如何处理 HTTP 请求和发送响应
4. 路径参数的使用方法

**接下来可以学习:**

- 第 3 章：HTTP 请求和响应对象的详细使用
- 处理 POST 请求和 JSON 数据
- 请求参数、查询字符串、请求头的处理
- Cookie 和 Session 管理

继续加油！🎯

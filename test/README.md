# Test Directory - Muduo网络库测试集合

这个目录包含了基于Muduo网络库的各种功能测试程序，涵盖了多线程、事件循环、网络通信、缓冲区管理等核心功能的使用示例。

## 📁 文件概览

| 文件名 | 功能描述 | 核心技术 |
|--------|----------|----------|
| `test_simple_thread.cpp` | 基础线程测试 | Thread, CurrentThread |
| `test_thread_eventloop.cpp` | 线程中的事件循环 | EventLoop, Thread |
| `threadpool_test.cpp` | 线程池功能测试 | ThreadPool, CountDownLatch |
| `timerfd_channel.cpp` | 定时器和Channel测试 | timerfd, Channel, EventLoop |
| `buffer_test.cpp` | 缓冲区操作测试 | Buffer读写操作 |
| `echo_server.cpp` | TCP回声服务器 | TcpServer, 多线程I/O |
| `echo_client.cpp` | TCP回声客户端 | TcpClient, 异步连接 |
| `test_eventloop.cpp` | 事件循环基础测试 | EventLoop, 定时任务 |

## 🚀 快速开始

### 编译要求
- C++11或更高版本
- Muduo网络库
- Linux系统（使用epoll、timerfd等Linux特有功能）

### 编译方法
```bash
# 单个文件编译示例
g++ -std=c++11 -o test_simple_thread test_simple_thread.cpp -lmuduo_net -lmuduo_base -lpthread

# 或使用CMake/Makefile（如果项目中有配置）
make test_simple_thread
```

## 📚 详细功能说明

### 1. 线程相关测试

#### `test_simple_thread.cpp` - 基础线程测试
**功能**: 演示Muduo Thread类的基本使用
- 创建新线程执行指定函数
- 线程同步（join）
- 线程ID获取和显示

**运行效果**:
```
主线程启动. 进程ID: 12345, 线程ID: 12345
线程对象已创建
正在启动新线程...
主线程等待新线程结束...
进入myThreadFunction. 进程ID: 12345, 线程ID: 12346
myThreadFunction执行完毕
新线程已结束. 主线程退出
```

#### `threadpool_test.cpp` - 线程池测试
**功能**: 展示ThreadPool的任务调度能力
- 创建固定数量的工作线程
- 任务队列管理
- 并发任务执行

**关键特性**:
- 线程池大小：3个工作线程
- 队列容量：5个任务
- 任务数量：10个（展示队列管理）

### 2. 事件循环测试

#### `test_eventloop.cpp` - 基础事件循环
**功能**: EventLoop的基本使用和定时任务
- 创建事件循环实例
- 延时回调执行（runAfter）
- 线程归属检查

#### `test_thread_eventloop.cpp` - 多线程事件循环
**功能**: 在子线程中运行EventLoop
- 线程安全的EventLoop创建
- 跨线程事件循环管理
- 线程本地EventLoop检查

#### `timerfd_channel.cpp` - 定时器和Channel
**功能**: 展示定时器fd与Channel的结合使用
- 创建timerfd定时器
- Channel事件监听
- 周期性定时回调

**技术要点**:
- 使用Linux timerfd API
- Channel读事件监听
- 定时器状态重置

### 3. 网络通信测试

#### `echo_server.cpp` - TCP回声服务器
**功能**: 高性能的多线程TCP服务器
- 监听端口9988
- 3个I/O线程处理并发连接
- 简单的回声协议实现

**服务器特性**:
- 异步非阻塞I/O
- 连接建立/断开回调
- 高效的数据回传

#### `echo_client.cpp` - TCP回声客户端
**功能**: 异步TCP客户端实现
- 连接到localhost:9988
- 自动消息发送和接收
- 连接状态管理

**客户端特性**:
- 异步连接建立
- 消息收发处理
- 连接断开自动退出

### 4. 缓冲区管理

#### `buffer_test.cpp` - Buffer操作测试
**功能**: Muduo Buffer类的完整使用示例
- 数据追加（append）
- 数据窥视（peek）
- 部分数据提取（retrieve）
- 完整数据提取（retrieveAllAsString）

**缓冲区状态跟踪**:
- 可读字节数（readableBytes）
- 可写字节数（writableBytes）
- 预留空间（prependableBytes）

## 🔧 使用建议

### 学习顺序推荐
1. **基础概念**: `test_simple_thread.cpp` → `test_eventloop.cpp`
2. **进阶功能**: `threadpool_test.cpp` → `test_thread_eventloop.cpp`
3. **系统编程**: `timerfd_channel.cpp` → `buffer_test.cpp`
4. **网络编程**: `echo_server.cpp` + `echo_client.cpp`

### 运行测试
```bash
# 1. 先运行服务器（新终端）
./echo_server

# 2. 再运行客户端（另一个终端）
./echo_client

# 3. 单独测试其他功能
./test_simple_thread
./threadpool_test
./buffer_test
```

### 调试技巧
- 使用`LOG_INFO`查看详细日志
- 观察进程ID和线程ID变化
- 监控网络连接状态
- 检查缓冲区状态变化

## 🛠️ 技术栈

- **核心库**: Muduo网络库
- **系统调用**: epoll, timerfd, socket
- **并发模型**: Reactor模式 + 线程池
- **内存管理**: RAII + 智能指针
- **日志系统**: Muduo Logging

## 📖 相关文档

- [Muduo网络库官方文档](https://github.com/chenshuo/muduo)
- [Linux网络编程](https://man7.org/linux/man-pages/)
- [C++并发编程](https://en.cppreference.com/w/cpp/thread)

## ⚠️ 注意事项

1. **平台依赖**: 这些测试程序依赖Linux特有的API（epoll、timerfd等）
2. **库依赖**: 需要预先安装Muduo库和相关依赖
3. **权限要求**: 某些测试可能需要特定的系统权限
4. **资源清理**: 运行网络测试后注意清理连接和端口占用

## 🐛 故障排除

### 常见编译错误
- **找不到头文件**: 检查Muduo库安装路径
- **链接错误**: 确保链接了正确的库文件（-lmuduo_net -lmuduo_base）
- **C++标准**: 使用C++11或更高版本

### 常见运行时错误
- **端口占用**: 确保9988端口未被占用
- **权限不足**: 某些系统调用可能需要特殊权限
- **库版本**: 检查Muduo库版本兼容性

---

*这些测试程序是学习Muduo网络库和Linux系统编程的绝佳资源，建议按顺序运行并仔细观察输出结果。*

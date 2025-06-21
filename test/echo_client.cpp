#include "muduo/net/TcpClient.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/base/Logging.h"  // 用于LOG_INFO
#include <cstdio>  // 用于printf
#include <string>  // 用于std::string
#include <unistd.h>

// 我们*单条连接*的TcpConnectionPtr（shared_ptr指向TcpConnection）
muduo::net::TcpConnectionPtr clientConnection;

// 示例连接回调
void onConnection(const muduo::net::TcpConnectionPtr& conn) {
  LOG_INFO << "客户端连接 " << (conn->connected() ? "建立" : "断开");

  if (conn->connected()) {
    // 连接建立时存储连接指针
    clientConnection = conn;
    LOG_INFO << "已连接到 " << conn->peerAddress().toIpPort();

    // 发送初始消息
    std::string message = "你好，来自客户端！\n";
    printf("发送: %s", message.c_str());
    conn->send(message);

  } else {
    // 连接断开，清空存储的指针
    clientConnection.reset();
    LOG_INFO << "已断开与 " << conn->peerAddress().toIpPort() << " 的连接";
    // 实际应用中可在此处调用loop.quit()或处理重连
    conn->getLoop()->quit();  // 简单示例：断开时退出循环
  }
}

// 示例消息回调（处理回声数据）
void onMessage(const muduo::net::TcpConnectionPtr& conn,
               muduo::net::Buffer* buf,
               muduo::Timestamp receiveTime) {
  LOG_INFO << "从 " << conn->name() 
           << " 接收到 " << buf->readableBytes() << " 字节";

  // 将回声数据转为字符串
  std::string msg = buf->retrieveAllAsString();
  printf("收到回声消息: %s", msg.c_str());

  // 简单示例：收到回声后发送下一条消息
  // 实际协议中应处理消息并决定是否响应
  // std::string nextMessage = "另一条消息...\n";
  // conn->send(nextMessage); // 按需持续发送
}

int main() {
  LOG_INFO << "main(): 进程ID = " << getpid();

  muduo::net::EventLoop loop;  // 客户端的事件循环

  // 服务器地址：localhost (127.0.0.1)，端口9988
  muduo::net::InetAddress serverAddr("127.0.0.1", 9988);

  // 创建TcpClient实例
  muduo::net::TcpClient client(&loop, serverAddr, "EchoClient");

  // --- 配置客户端 ---
  // 设置回调
  client.setConnectionCallback(onConnection);
  client.setMessageCallback(onMessage);

  // 可选：启用连接失败/断开自动重连
  // client.enableRetry();
  // LOG_INFO << "已启用重连";

  // --- 启动连接过程 ---
  printf("启动客户端，正在连接到 %s...\n", serverAddr.toIpPort().c_str());
  client.connect();

  // --- 运行客户端循环 ---
  printf("运行客户端循环...\n");
  loop.loop();  // 阻塞直到调用loop.quit()

  // loop.loop()在连接关闭且onConnection调用loop->quit()后退出

  printf("客户端已停止。main()退出。\n");

  return 0;
} 
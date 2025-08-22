#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/base/Logging.h"
#include <cstdio>
#include <unistd.h> // for getpid()

// 连接状态回调
void onConnection(const muduo::net::TcpConnectionPtr& conn) {
  if (conn->connected()) {
    LOG_INFO << "连接建立: " << conn->name();
  } else {
    LOG_INFO << "连接断开: " << conn->name();
  }
}

// 消息处理回调（回声逻辑）
void onMessage(const muduo::net::TcpConnectionPtr& conn,
               muduo::net::Buffer* buf,
               muduo::Timestamp receiveTime) {
  LOG_INFO << "从连接" << conn->name() << "收到" << buf->readableBytes() << "字节";
  conn->send(buf); // 高效回传数据
}

int main() {
  LOG_INFO << "主进程ID: " << getpid();

  muduo::net::EventLoop loop; // 接收器循环
  muduo::net::InetAddress listenAddr(9988); // 监听本地9988端口
  muduo::net::TcpServer server(&loop, listenAddr, "EchoServer");

  // 配置服务器
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  server.setThreadNum(3); // 使用3个I/O线程

  printf("启动回声服务器（端口9988，3个I/O线程）...\n");
  server.start();

  printf("运行接收器循环...\n");
  loop.loop(); // 阻塞运行

  printf("服务器停止.\n");
  return 0;
} 
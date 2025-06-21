#include "muduo/net/Buffer.h"
#include <cstdio>
#include <string>
#include <cstring>

int main() {
  muduo::net::Buffer buffer;
  printf("初始状态: 可读字节=%zu, 可写字节=%zu, 可预留空间=%zu\n",
         buffer.readableBytes(), buffer.writableBytes(), buffer.prependableBytes());

  // 追加数据
  const char* data1 = "你好，";
  buffer.append(data1, strlen(data1));
  printf("追加'你好，'后: 可读=%zu, 可写=%zu, 可预留=%zu\n",
         buffer.readableBytes(), buffer.writableBytes(), buffer.prependableBytes());

  const char* data2 = "世界！\n";
  buffer.append(data2, strlen(data2));
  printf("追加'世界！\\n'后: 可读=%zu, 可写=%zu, 可预留=%zu\n",
         buffer.readableBytes(), buffer.writableBytes(), buffer.prependableBytes());

  // 窥视数据
  printf("窥视数据: %s (首%zu字节)\n", buffer.peek(), buffer.readableBytes());

  // 提取部分数据
  size_t 提取长度 = 7; // "你好，"的字节长度
  std::string 部分数据(buffer.peek(), 提取长度);
  buffer.retrieve(提取长度);
  printf("提取%zu字节: '%s'\n", 提取长度, 部分数据.c_str());
  printf("提取后: 可读=%zu, 可写=%zu, 可预留=%zu\n",
         buffer.readableBytes(), buffer.writableBytes(), buffer.prependableBytes());

  // 提取剩余数据
  std::string 剩余数据 = buffer.retrieveAllAsString();
  printf("提取剩余数据: '%s'\n", 剩余数据.c_str());
  printf("全部提取后: 可读=%zu, 可写=%zu, 可预留=%zu\n",
         buffer.readableBytes(), buffer.writableBytes(), buffer.prependableBytes());

  return 0;
} 
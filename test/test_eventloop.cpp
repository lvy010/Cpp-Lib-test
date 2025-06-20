#include "muduo/net/EventLoop.h"
#include <cstdio>
#include <unistd.h>

void myCallback() {
  printf("回调执行! pid = %d\n", getpid());
}

int main() {
  printf("主线程启动. pid = %d\n", getpid());

  muduo::net::EventLoop loop;
  printf("EventLoop 对象 'loop' 已创建\n");

  assert(muduo::net::EventLoop::getEventLoopOfCurrentThread() == &loop);
  printf("确认 'loop' 属于主线程\n");

  printf("调度 2 秒后执行 myCallback...\n");
  loop.runAfter(2.0, myCallback);

  printf("启动事件循环...\n");
  loop.loop();
  printf("事件循环结束\n");

  return 0;
}
#include "muduo/net/EventLoop.h"
#include "muduo/base/Thread.h"
#include <cstdio>
#include <assert.h>
#include <unistd.h>

void threadFunc() {
  printf("线程函数启动. pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
  assert(muduo::net::EventLoop::getEventLoopOfCurrentThread() == NULL);

  muduo::net::EventLoop loop;
  printf("新线程中创建 EventLoop\n");
  assert(muduo::net::EventLoop::getEventLoopOfCurrentThread() == &loop);

  printf("启动新线程事件循环...\n");
  loop.loop();
  printf("新线程循环结束\n");
}

int main() {
  printf("主线程启动. pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
  
  muduo::Thread thread(threadFunc);
  printf("创建新线程...\n");
  thread.start();
  thread.join();
  printf("子线程结束\n");
  
  return 0;
}

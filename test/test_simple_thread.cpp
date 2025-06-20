#include <cstdio>
#include <unistd.h> // getpid
#include "muduo/base/CurrentThread.h" // muduo::CurrentThread::tid()
#include "muduo/base/Thread.h"

// 该函数将在新线程中运行
void myThreadFunction() {
  printf("进入myThreadFunction. 进程ID: %d, 线程ID: %d\n",
         getpid(), muduo::CurrentThread::tid());
  // 执行某些工作...例如休眠
  sleep(3);
  printf("myThreadFunction执行完毕\n");
}

int main() {
  printf("主线程启动. 进程ID: %d, 线程ID: %d\n",
         getpid(), muduo::CurrentThread::tid());

  // 1. 创建Thread对象，传入要运行的函数
  muduo::Thread thread(myThreadFunction, "MyWorkerThread");
  printf("线程对象已创建\n");

  // 2. 启动新线程
  printf("正在启动新线程...\n");
  thread.start();

  // 主线程可在此执行其他工作
  // printf("主线程正在处理其他任务...\n");
  // sleep(1);

  // 3. 等待新线程结束
  printf("主线程等待新线程结束...\n");
  thread.join();
  printf("新线程已结束. 主线程退出\n");

  return 0;
}

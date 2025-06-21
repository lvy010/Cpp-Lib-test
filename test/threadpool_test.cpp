#include "muduo/base/ThreadPool.h"
#include "muduo/base/CurrentThread.h" 
#include "muduo/base/CountDownLatch.h" 
#include <cstdio>
#include <string>
#include <unistd.h>
#include <functional>

// 工作线程中运行的函数
void print(const std::string& msg) {
  printf("线程池任务: %s, 进程ID: %d, 线程ID: %d\n",
         msg.c_str(), getpid(), muduo::CurrentThread::tid());
  sleep(1); // 模拟耗时操作
}

int main() {
  printf("主线程启动. 进程ID: %d, 线程ID: %d\n", getpid(), muduo::CurrentThread::tid());

  muduo::ThreadPool pool("MyWorkerPool"); // 1. 创建线程池
  pool.setMaxQueueSize(5);               // 设置队列最大容量为5
  pool.start(3);                          // 2. 启动3个工作线程

  sleep(1); // 等待线程启动
  printf("线程池已启动.\n");

  // 3. 提交10个任务
  for (int i = 0; i < 10; ++i) {
    char task_msg[32];
    snprintf(task_msg, sizeof task_msg, "任务%d", i);
    pool.run(std::bind(print, std::string(task_msg))); // 提交任务
  }
  printf("任务提交完成.\n");

  sleep(10); // 等待任务执行
  pool.stop(); // 4. 停止线程池
  printf("线程池已停止.\n");
  return 0;
} 
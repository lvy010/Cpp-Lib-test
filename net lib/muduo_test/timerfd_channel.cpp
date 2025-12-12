#include <sys/timerfd.h>
#include <unistd.h>
#include <cstdio>
#include "muduo/net/EventLoop.h"
#include "muduo/net/Channel.h"
#include <functional>

// 步骤1：创建定时器FD
int createTimerfd() {
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, 
                                  TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) {
        perror("timerfd_create error");
    }
    return timerfd;
}

// timerfd 需要全局或静态变量以便回调访问
int timerfd = -1;

// 步骤2：定时器回调函数
void timerCallback(muduo::Timestamp receiveTime) {
    printf("定时触发时间: %s\n", 
          receiveTime.toFormattedString().c_str());
    uint64_t expirations;
    ::read(timerfd, &expirations, sizeof expirations); // 必须读取以重置状态
}

int main() {
    muduo::net::EventLoop loop;  // 事件循环实例
    
    // 步骤3：创建并配置Channel
    timerfd = createTimerfd();
    muduo::net::Channel timerChannel(&loop, timerfd);
    
    // 步骤4：设置读回调
    timerChannel.setReadCallback(
        std::bind(timerCallback, std::placeholders::_1));
    
    // 步骤5：启用读事件监控
    timerChannel.enableReading();
    
    // 步骤6：配置定时器参数
    struct itimerspec spec{
        .it_interval = {1, 0},  // 每秒触发
        .it_value = {1, 0}      // 首次触发在1秒后
    };
    ::timerfd_settime(timerfd, 0, &spec, nullptr);
    
    loop.loop();  // 进入事件循环
    ::close(timerfd);
    return 0;
} 
#include <pplx/pplxtasks.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    pplx::cancellation_token_source cts;
    pplx::cancellation_token token = cts.get_token();

    pplx::task<void> long_running_task = pplx::create_task([token]() {
        for (int i = 0; i < 5; ++i) {
            if (token.is_canceled()) {
                std::cout << "任务: 请求取消，提前停止！" << std::endl;
                throw pplx::task_canceled();
            }
            std::cout << "任务: 仍在工作... (" << i+1 << "/5)" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        std::cout << "任务: 未取消完成。" << std::endl;
    }, token);

    std::cout << "主线程: 启动一个长时间运行的任务。" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    std::cout << "主线程: 请求取消！" << std::endl;
    cts.cancel();

    try {
        long_running_task.get();
    } catch (const pplx::task_canceled&) {
        std::cout << "主线程: 捕获到task_canceled异常。" << std::endl;
    }

    return 0;
}

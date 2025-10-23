#include <pplx/pplxtasks.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    pplx::task_completion_event<int> tce;
    pplx::task<int> controlled_task(tce);

    std::thread background_worker([tce]() mutable {
        std::cout << "后台工作线程正在处理一些长时间任务..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "后台工作线程完成，设置TCE为42。" << std::endl;
        tce.set(42);
    });

    std::cout << "主线程继续，通过.then()等待controlled_task。" << std::endl;

    auto t = controlled_task.then([](int result) {
        std::cout << "受控任务完成，结果: " << result << std::endl;
    });

    background_worker.join();
    t.wait();

    return 0;
}

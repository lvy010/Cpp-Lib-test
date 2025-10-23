#include "cpprest/http_client.h"
#include "cpprest/uri.h"
#include "cpprest/http_msg.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main() {
    pplx::task<int> calculation_task = pplx::create_task([]() {
        std::cout << "在后台任务中计算..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 10 + 20;
    });

    std::cout << "主线程在计算任务运行时继续执行。" << std::endl;

    int result = calculation_task.get();
    std::cout << "计算任务完成。结果: " << result << std::endl;

    pplx::task<utility::string_t> greeting_task = pplx::task_from_result(utility::string_t(U("你好，PPLX！")));
    utility::string_t greeting = greeting_task.get();
    std::cout << "预完成任务的结果: " << greeting << std::endl;

    return 0;
}

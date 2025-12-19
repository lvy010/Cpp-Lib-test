// main.cc
#include <drogon/drogon.h>
#include <iostream>

int main()
{
    // 设置日志级别
    drogon::app().setLogLevel(trantor::Logger::kInfo);
    
    // 配置监听地址和端口
    drogon::app().addListener("127.0.0.1", 8848);
    
    // 设置线程数
    drogon::app().setThreadNum(4);
    
    // 输出服务器信息
    std::cout << "==========================================" << std::endl;
    std::cout << "Server running on http://127.0.0.1:8848" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Test URLs:" << std::endl;
    std::cout << "  - http://127.0.0.1:8848/MyHomeController/hello" << std::endl;
    std::cout << "  - http://127.0.0.1:8848/MyHomeController/welcome/Alice" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    // 启动 Drogon 应用程序
    drogon::app().run();
    
    return 0;
}

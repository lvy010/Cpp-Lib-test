#include "cpprest/http_client.h"
#include "cpprest/uri.h"
#include "cpprest/http_msg.h"
#include <iostream>
#include <string>

int main() {
    using web::http::client::http_client;
    using web::http::http_request;
    using web::http::methods;
    using web::uri;
    using web::uri_builder;

    uri base_api_uri(U("https://httpbin.org"));
    http_client client(base_api_uri);

    uri_builder uri_path_builder(U("/get"));
    uri_path_builder.append_query(U("city"), U("London"));
    uri_path_builder.append_query(U("days"), 3);

    http_request request(methods::GET);
    request.set_request_uri(uri_path_builder.to_uri());

    std::cout << "异步发送请求..." << std::endl;

    auto t = client.request(request)
        .then([](web::http::http_response response) {
            std::cout << "收到响应（在延续线程中）！" << std::endl;
            std::cout << "状态码: " << response.status_code() << std::endl;
            return response.extract_string();
        })
        .then([](utility::string_t body) {
            std::cout << "正文提取完成（在另一个延续线程中）！" << std::endl;
            std::cout << "响应正文:\n" << body << std::endl;
        })
        .then([](pplx::task<void> previous_task) {
            try {
                previous_task.get();
            } catch (const web::http::http_exception& e) {
                std::cout << "捕获到HTTP异常: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cout << "捕获到一般异常: " << e.what() << std::endl;
            }
        });

    std::cout << "主线程在网络操作进行时可以自由处理其他工作。" << std::endl;

    t.wait();

    return 0;
}

// MyHomeController.cc
#include "MyHomeController.h"
#include <drogon/HttpResponse.h>

void MyHomeController::hello(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) const
{
    // 创建一个新的 HTTP 响应对象
    auto resp = drogon::HttpResponse::newHttpResponse();
    
    // 设置响应内容
    resp->setBody("Hello from Drogon Controller!");
    
    // 设置内容类型
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    
    // 通过回调函数发送响应
    callback(resp);
}

void MyHomeController::welcome(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback,
                               const std::string &name) const
{
    // 创建响应
    auto resp = drogon::HttpResponse::newHttpResponse();
    
    // 使用路径参数构造响应内容
    resp->setBody("Welcome, " + name + "!");
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    
    // 发送响应
    callback(resp);
}

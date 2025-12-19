// MyHomeController.h
#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class MyHomeController : public drogon::HttpController<MyHomeController>
{
  public:
    METHOD_LIST_BEGIN
    // 添加方法列表
    METHOD_ADD(MyHomeController::hello, "/hello", Get);
    METHOD_ADD(MyHomeController::welcome, "/welcome/{name}", Get);
    METHOD_LIST_END

    // 处理函数声明
    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) const;
    
    void welcome(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &name) const;
};

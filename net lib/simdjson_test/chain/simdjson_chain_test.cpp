#include <simdjson.h>
#include <iostream>

int main() {
    simdjson::ondemand::parser parser;
    // 嵌套结构JSON
    simdjson::padded_string json_data = R"({"user": {"address": {"city": "Toronto"}}})"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(json_data).get(doc);

    if (error) { /* 处理迭代错误 */ return 1; }

    std::string_view city_name;
    // 串联多个访问和最终值提取
    error = doc["user"]["address"]["city"].get_string().get(city_name);

    if (error) { // 仅在链式末端检查错误
        std::cerr << "访问城市错误: " << error << std::endl;
        // 错误可能是NO_SUCH_FIELD（user、address或city缺失）
        // 或INCORRECT_TYPE（user、address或city类型不符）
        return EXIT_FAILURE;
    }

    std::cout << "城市: " << city_name << std::endl;

    return EXIT_SUCCESS;
} 
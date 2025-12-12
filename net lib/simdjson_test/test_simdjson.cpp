#include <simdjson.h>
#include <iostream>

int main() {
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = R"({\"message\": \"hello world\", \"status\": true})"_padded;
    simdjson::simdjson_result<simdjson::ondemand::document> result = parser.iterate(json_data);

    if (result.error()) {
        std::cerr << "解析初始化失败: " << result.error() << std::endl;
        return EXIT_FAILURE;
    }

    simdjson::ondemand::document doc = std::move(result.value());

    // 6. 检查文档根节点类型
    simdjson::simdjson_result<simdjson::ondemand::json_type> root_type_result = doc.type();
    if (root_type_result.error()) {
         std::cerr << "获取根类型错误: " << root_type_result.error() << std::endl;
         return EXIT_FAILURE;
    }

    simdjson::ondemand::json_type root_type = root_type_result.value();

    if (root_type == simdjson::ondemand::json_type::object) {
        std::cout << "根节点是 JSON 对象。" << std::endl;

        // 7. 以对象形式访问根节点
        simdjson::simdjson_result<simdjson::ondemand::object> obj_result = doc.get_object();
        if (obj_result.error()) {
            std::cerr << "获取根对象错误: " << obj_result.error() << std::endl;
            return EXIT_FAILURE;
        }
        simdjson::ondemand::object root_object = obj_result.value();

        std::cout << "成功访问根对象。" << std::endl;

        // 后续章节将学习如何使用此'root_object'
        // 访问"message"和"status"等字段

    } else {
        std::cout << "根节点不是对象，类型代码: " << int(root_type) << std::endl;
    }

    // 重要提示：'doc'对象、解析器和 json_data 必须保持有效
    // 只要仍在使用从'doc'派生的任何数据

    return EXIT_SUCCESS;
} 
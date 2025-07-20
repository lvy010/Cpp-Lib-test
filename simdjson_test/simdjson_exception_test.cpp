#include <simdjson.h>
#include <iostream>

int main() {
    simdjson::ondemand::parser parser;
    
    // 测试多个错误JSON
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {
            R"({"data": [1, 2,], "status": "incomplete")",
            "缺少闭合括号且有语法错误（多余逗号）"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok"})",
            "正确的JSON"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok",})",
            "对象末尾多余逗号"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok", "extra":})",
            "缺少值的键值对"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok", "number": 123.456.789})",
            "无效的数字格式"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok", "string": "unclosed string)",
            "未闭合的字符串"
        }
    };

    std::cout << "=== simdjson 异常处理测试 ===\n\n";

    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        std::cout << "测试 " << (i + 1) << ": " << test_case.second << std::endl;
        std::cout << "JSON: " << test_case.first << std::endl;
        
        simdjson::padded_string json_data = simdjson::padded_string(test_case.first);

        try {
            // 调用iterate，失败时抛出异常
            simdjson::ondemand::document doc = parser.iterate(json_data);

            // 若到达此处说明迭代成功
            std::cout << "解析成功 ✓" << std::endl;
            
            // 尝试获取对象字段及其字符串值
            // 直接使用operator[]和get_string，失败时抛出异常
            std::string_view status_sv = doc["status"].get_string();
            std::cout << "  状态: " << status_sv << std::endl;

            // 尝试访问数组
            auto data_array = doc["data"];
            if (!data_array.error()) {
                std::cout << "  成功访问 'data' 数组" << std::endl;
                
                // 尝试遍历数组
                for (auto element : data_array) {
                    auto element_result = element.get_int64();
                    if (!element_result.error()) {
                        int64_t value = element_result.value();
                        std::cout << "    数组元素: " << value << std::endl;
                    }
                }
            }

        } catch (const simdjson::simdjson_error& e) {
            // 捕获simdjson特定错误
            std::cout << "Simdjson错误: " << e.what() << std::endl;
            // 可从异常获取具体错误码
            std::cout << "错误代码: " << e.error() << std::endl;
        } catch (const std::exception& e) {
            // 捕获其他标准异常
            std::cout << "标准异常: " << e.what() << std::endl;
        }
        
        std::cout << std::string(50, '-') << std::endl;
    }

    return EXIT_SUCCESS;
} 
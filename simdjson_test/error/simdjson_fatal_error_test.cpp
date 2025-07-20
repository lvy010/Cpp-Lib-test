#include <simdjson.h>
#include <iostream>

int main() {
    simdjson::ondemand::parser parser;
    
    // 测试多个不同类型的错误JSON
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {
            R"({"data": [1, 2 3], "status": true})",
            "缺少逗号导致TAPE_ERROR（致命错误）"
        },
        {
            R"({"data": [1, 2, 3], "status": true})",
            "正确的JSON（用于对比）"
        },
        {
            R"({"data": [1, 2, 3], "status": true, "extra":})",
            "缺少值的键值对（致命错误）"
        },
        {
            R"({"data": [1, 2, 3], "status": true, "string": "unclosed)",
            "未闭合的字符串（致命错误）"
        },
        {
            R"({"data": [1, 2, 3], "status": true, "number": 123.456.789})",
            "无效的数字格式（致命错误）"
        },
        {
            R"({"data": [1, 2, 3], "status": true, "nested": {"key": "value",}})",
            "嵌套对象末尾多余逗号（致命错误）"
        }
    };

    std::cout << "=== simdjson 致命错误测试 ===\n\n";

    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        std::cout << "测试 " << (i + 1) << ": " << test_case.second << std::endl;
        std::cout << "JSON: " << test_case.first << std::endl;
        
        simdjson::padded_string json_data = simdjson::padded_string(test_case.first);

        // 创建新的解析器实例（避免致命错误影响）
        simdjson::ondemand::parser parser;
        simdjson::ondemand::document doc;
        simdjson::error_code error = parser.iterate(json_data).get(doc);

        if (error) {
            std::cout << "初始迭代失败: " << simdjson::error_message(error) << " (错误码: " << error << ")" << std::endl;
            if (simdjson::is_fatal(error)) {
                std::cout << "  → 这是致命错误，解析器状态已损坏" << std::endl;
            } else {
                std::cout << "  → 这是可恢复错误" << std::endl;
            }
        } else {
            std::cout << "初始迭代成功 ✓" << std::endl;
            
            // 尝试访问数组
            simdjson::ondemand::array data_array;
            error = doc["data"].get_array().get(data_array);

            if (error) {
                std::cout << "访问'data'数组失败: " << simdjson::error_message(error) << " (错误码: " << error << ")" << std::endl;
                if (simdjson::is_fatal(error)) {
                    std::cout << "  → 这是致命错误，无法继续使用解析器/文档" << std::endl;
                    std::cout << "  → 解析器和文档现在处于无效状态" << std::endl;
                    std::cout << "  → 不要继续使用'doc'或'parser'，除非使用current_location()" << std::endl;
                    std::cout << "  → 若需解析新文档，应创建新解析器" << std::endl;
                } else {
                    std::cout << "  → 这是可恢复错误，可能继续处理文档其他部分" << std::endl;
                    std::cout << "  → 对于特定错误（如NO_SUCH_FIELD），可以继续访问doc[\"status\"]" << std::endl;
                    
                    // 尝试访问其他字段
                    auto status_result = doc["status"];
                    if (!status_result.error()) {
                        auto bool_result = status_result.get_bool();
                        if (!bool_result.error()) {
                            bool status_value = bool_result.value();
                            std::cout << "  状态: " << (status_value ? "true" : "false") << std::endl;
                        }
                    }
                }
            } else {
                std::cout << "成功访问数据数组 ✓" << std::endl;
                
                // 尝试遍历数组
                int count = 0;
                for (auto element : data_array) {
                    auto element_result = element.get_int64();
                    if (!element_result.error()) {
                        int64_t value = element_result.value();
                        std::cout << "  数组元素[" << count++ << "]: " << value << std::endl;
                    }
                }
                
                // 尝试访问其他字段
                auto status_result = doc["status"];
                if (!status_result.error()) {
                    auto bool_result = status_result.get_bool();
                    if (!bool_result.error()) {
                        bool status_value = bool_result.value();
                        std::cout << "  状态: " << (status_value ? "true" : "false") << std::endl;
                    }
                }
            }
        }
        
        std::cout << std::string(60, '-') << std::endl;
    }

    // 演示致命错误后的解析器状态
    std::cout << "\n=== 致命错误后的解析器状态演示 ===\n";
    
    simdjson::ondemand::parser demo_parser;
    simdjson::padded_string bad_json = R"({"data": [1, 2 3], "status": true})"_padded;
    
    simdjson::ondemand::document doc1;
    simdjson::error_code error1 = demo_parser.iterate(bad_json).get(doc1);
    
    if (error1) {
        std::cout << "第一次解析失败: " << simdjson::error_message(error1) << std::endl;
        
        if (simdjson::is_fatal(error1)) {
            std::cout << "致命错误！解析器状态已损坏\n";
            
            // 尝试使用损坏的解析器解析新文档（会失败）
            simdjson::padded_string good_json = R"({"data": [1, 2, 3], "status": true})"_padded;
            simdjson::ondemand::document doc2;
            simdjson::error_code error2 = demo_parser.iterate(good_json).get(doc2);
            
            if (error2) {
                std::cout << "使用损坏的解析器解析正确JSON也失败: " << simdjson::error_message(error2) << std::endl;
            }
            
            // 创建新解析器（正确做法）
            std::cout << "创建新解析器...\n";
            simdjson::ondemand::parser new_parser;
            simdjson::ondemand::document doc3;
            simdjson::error_code error3 = new_parser.iterate(good_json).get(doc3);
            
            if (!error3) {
                std::cout << "新解析器成功解析正确JSON ✓\n";
            }
        }
    }

    return EXIT_SUCCESS;
} 
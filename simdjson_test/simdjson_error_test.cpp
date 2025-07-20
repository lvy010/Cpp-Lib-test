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
            R"({"data": [1, 2, 3], "status": "ok")",
            "正确的JSON"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok",})",
            "对象末尾多余逗号"
        },
        {
            R"({"data": [1, 2, 3], "status": "ok"})",
            "正确的JSON（用于对比）"
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

    std::cout << "=== simdjson 错误处理测试 ===\n\n";

    for (size_t i = 0; i < test_cases.size(); ++i) {
        const auto& test_case = test_cases[i];
        std::cout << "测试 " << (i + 1) << ": " << test_case.second << std::endl;
        std::cout << "JSON: " << test_case.first << std::endl;
        
        simdjson::padded_string json_data = simdjson::padded_string(test_case.first);
        auto doc_result = parser.iterate(json_data);

        if (doc_result.error()) {
            std::cout << "解析失败: " << simdjson::error_message(doc_result.error()) << std::endl;
            std::cout << "错误代码: " << doc_result.error() << std::endl;
        } else {
            std::cout << "解析成功 ✓" << std::endl;
            
            // 尝试访问一些字段来验证解析
            try {
                auto& doc = doc_result.value();
                auto data_array = doc["data"];
                if (!data_array.error()) {
                    std::cout << "  成功访问 'data' 字段" << std::endl;
                }
                
                auto status = doc["status"];
                if (!status.error()) {
                    std::string status_str;
                    if (status.get_string().get(status_str) == simdjson::SUCCESS) {
                        std::cout << "  成功访问 'status' 字段: " << status_str << std::endl;
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "  访问字段时出错: " << e.what() << std::endl;
            }
        }
        
        std::cout << std::string(50, '-') << std::endl;
    }

    return EXIT_SUCCESS;
} 
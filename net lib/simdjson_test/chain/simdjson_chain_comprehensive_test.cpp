#include <simdjson.h>
#include <iostream>
#include <vector>

void test_successful_chain() {
    std::cout << "\n=== 测试成功的链式调用 ===" << std::endl;
    
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = R"({
        "user": {
            "name": "张三",
            "age": 30,
            "address": {
                "city": "北京",
                "street": "长安街",
                "postal_code": "100000"
            },
            "hobbies": ["读书", "游泳", "编程"]
        }
    })"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(json_data).get(doc);

    if (error) {
        std::cerr << "解析错误: " << error << std::endl;
        return;
    }

    // 测试字符串链式调用
    std::string_view name, city, street;
    error = doc["user"]["name"].get_string().get(name);
    if (!error) {
        std::cout << "用户名: " << name << std::endl;
    }

    error = doc["user"]["address"]["city"].get_string().get(city);
    if (!error) {
        std::cout << "城市: " << city << std::endl;
    }

    error = doc["user"]["address"]["street"].get_string().get(street);
    if (!error) {
        std::cout << "街道: " << street << std::endl;
    }

    // 测试数字链式调用
    int64_t age;
    error = doc["user"]["age"].get_int64().get(age);
    if (!error) {
        std::cout << "年龄: " << age << std::endl;
    }

    // 测试数组链式调用
    simdjson::ondemand::array hobbies;
    error = doc["user"]["hobbies"].get_array().get(hobbies);
    if (!error) {
        std::cout << "爱好: ";
        for (auto hobby : hobbies) {
            std::string_view hobby_str;
            if (hobby.get_string().get(hobby_str) == simdjson::SUCCESS) {
                std::cout << hobby_str << " ";
            }
        }
        std::cout << std::endl;
    }
}

void test_missing_field_chain() {
    std::cout << "\n=== 测试缺失字段的链式调用 ===" << std::endl;
    
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = R"({
        "user": {
            "name": "李四"
        }
    })"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(json_data).get(doc);

    if (error) {
        std::cerr << "解析错误: " << error << std::endl;
        return;
    }

    // 尝试访问不存在的字段
    std::string_view city;
    error = doc["user"]["address"]["city"].get_string().get(city);
    
    if (error == simdjson::NO_SUCH_FIELD) {
        std::cout << "字段不存在错误: " << error << " (address字段缺失)" << std::endl;
    } else if (error) {
        std::cout << "其他错误: " << error << std::endl;
    } else {
        std::cout << "城市: " << city << std::endl;
    }
}

void test_type_mismatch_chain() {
    std::cout << "\n=== 测试类型不匹配的链式调用 ===" << std::endl;
    
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = R"({
        "user": {
            "name": 123,
            "age": "不是数字"
        }
    })"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(json_data).get(doc);

    if (error) {
        std::cerr << "解析错误: " << error << std::endl;
        return;
    }

    // 尝试将数字作为字符串访问
    std::string_view name;
    error = doc["user"]["name"].get_string().get(name);
    
    if (error == simdjson::INCORRECT_TYPE) {
        std::cout << "类型错误: " << error << " (name字段是数字，不是字符串)" << std::endl;
    } else if (error) {
        std::cout << "其他错误: " << error << std::endl;
    } else {
        std::cout << "用户名: " << name << std::endl;
    }

    // 尝试将字符串作为数字访问
    int64_t age;
    error = doc["user"]["age"].get_int64().get(age);
    
    if (error == simdjson::INCORRECT_TYPE) {
        std::cout << "类型错误: " << error << " (age字段是字符串，不是数字)" << std::endl;
    } else if (error) {
        std::cout << "其他错误: " << error << std::endl;
    } else {
        std::cout << "年龄: " << age << std::endl;
    }
}

void test_deep_nested_chain() {
    std::cout << "\n=== 测试深层嵌套的链式调用 ===" << std::endl;
    
    simdjson::ondemand::parser parser;
    simdjson::padded_string json_data = R"({
        "level1": {
            "level2": {
                "level3": {
                    "level4": {
                        "level5": {
                            "value": "深层嵌套的值"
                        }
                    }
                }
            }
        }
    })"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(json_data).get(doc);

    if (error) {
        std::cerr << "解析错误: " << error << std::endl;
        return;
    }

    std::string_view deep_value;
    error = doc["level1"]["level2"]["level3"]["level4"]["level5"]["value"].get_string().get(deep_value);
    
    if (!error) {
        std::cout << "深层嵌套值: " << deep_value << std::endl;
    } else {
        std::cout << "访问深层嵌套值错误: " << error << std::endl;
    }
}

int main() {
    std::cout << "simdjson 链式调用测试" << std::endl;
    std::cout << "=====================" << std::endl;

    try {
        test_successful_chain();
        test_missing_field_chain();
        test_type_mismatch_chain();
        test_deep_nested_chain();
        
        std::cout << "\n所有测试完成！" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} 
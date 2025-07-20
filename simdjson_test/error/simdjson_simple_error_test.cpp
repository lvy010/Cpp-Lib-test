#include <simdjson.h>
#include <iostream>
#include <string_view> // 用于std::string_view

int main() {
    simdjson::ondemand::parser parser;
    // 未闭合的字符串JSON
       simdjson::padded_string bad_json = R"({"message": "hello world, "status": true})"_padded;

    simdjson::ondemand::document doc;
    simdjson::error_code error = parser.iterate(bad_json).get(doc);

    if (error) {
        std::cerr << "解析失败: " << error << std::endl;

        // 尝试获取错误位置
        simdjson::simdjson_result<const char*> loc_result = doc.current_location();

        if (loc_result.error()) {
            std::cerr << "无法获取错误位置: " << loc_result.error() << std::endl;
        } else {
            const char* error_ptr = loc_result.value();
            size_t error_pos = error_ptr - bad_json.data();

            std::cerr << "错误发生在字节位置: " << error_pos << std::endl;

            // 可选打印错误位置附近的片段
            size_t snippet_len = 20;
            size_t start_pos = (error_pos > snippet_len) ? error_pos - snippet_len : 0;
            size_t end_pos = std::min(error_pos + snippet_len, bad_json.size());
            std::string_view snippet(bad_json.data() + start_pos, end_pos - start_pos);

            std::cerr << "片段: ..." << snippet << "..." << std::endl;
            // 注意：若使用padded_string_view，error_ptr - view.data()给出索引
        }

        return EXIT_FAILURE;
    }

    std::cout << "解析成功！" << std::endl; // bad_json不会到达此处

    return EXIT_SUCCESS;
} 
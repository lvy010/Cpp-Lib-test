#include <simdjson.h>
#include <iostream>

int main() {
    simdjson::padded_string ndjson_data = R"(
        {"user":"Alice", "id":1}
        {"user":"Bob", "id":2}
        {"user":"Charlie", "id":3}
    )"_padded;

    simdjson::ondemand::parser parser;
    auto stream_result = parser.iterate_many(ndjson_data);

    if (stream_result.error()) {
        std::cerr << "文档流设置错误: " << stream_result.error() << std::endl;
        return EXIT_FAILURE;
    }
    simdjson::ondemand::document_stream doc_stream = std::move(stream_result.value());

    // 遍历流中每个文档
    int doc_count = 0;
    for (auto doc_result : doc_stream) {
        // 每个'doc_result'是simdjson_result<simdjson::ondemand::document_reference>

        // 访问文档前检查错误
        if (doc_result.error()) {
            std::cerr << "解析文档" << doc_count << "错误: " << doc_result.error() << std::endl;
            // 可选择继续或停止
            continue; // 跳过损坏文档
        }

        // 获取文档引用
        simdjson::ondemand::document_reference doc = doc_result.value();

        std::cout << "处理文档 " << doc_count << ":" << std::endl;

        // 可像常规文档一样使用'doc'对象访问字段
        auto user_result = doc["user"].get_string();
        if (!user_result.error()) {
            std::cout << "  用户: " << user_result.value() << std::endl;
        } else {
             std::cerr << "  获取用户字段错误: " << user_result.error() << std::endl;
        }

        auto id_result = doc["id"].get_int64();
         if (!id_result.error()) {
            std::cout << "  ID: " << id_result.value() << std::endl;
        } else {
             std::cerr << "  获取ID字段错误: " << id_result.error() << std::endl;
        }

        doc_count++;
    }

    std::cout << "已完成处理 " << doc_count << " 个文档。" << std::endl;

    // 'parser'和'ndjson_data'在循环期间需保持有效

    return EXIT_SUCCESS;
} 
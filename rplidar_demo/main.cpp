#include "sl_lidar_driver.h"          // For rplidar_response_measurement_node_hq_t
#include "dataunpacker/dataunpacker.h"  // For LIDARSampleDataUnpacker

#include <cstdio>

// 使用解包器命名空间
using namespace sl::rplidar::dataunpacker;

// 自定义监听器，处理解包后的数据
class MyLidarListener : public LIDARSampleDataListener {
public:
    void onHQNodeDecoded(_u64 timestamp_uS,
                         const rplidar_response_measurement_node_hq_t* node) override {
        printf("  新 HQ 节点: 时间戳=%llu us, 距离=%f mm, 角度=%f deg, 质量=%d\n",
               static_cast<unsigned long long>(timestamp_uS),
               static_cast<float>(node->distance_q2) / 4.0f,          // Q2 -> mm
               static_cast<float>(node->angle_z_q14) / 16384.0f,      // Q14 -> deg
               node->quality);
        if (node->flag & RPLIDAR_RESP_MEASUREMENT_HQ_SYNCBIT) {
            printf("  --- !!! 新扫描开始 !!! ---\n");
        }
    }

    void onHQNodeScanResetReq() override {
        printf("  --- 扫描重置请求 ---\n");
    }

    void onCustomSampleDataDecoded(_u8, _u32, const void*, size_t) override {}

    void onDecodingError(int errMsg, _u8 ansType, const void*, size_t) override {
        fprintf(stderr, "  解码错误: %d 对于 AnsType: %d\n", errMsg, ansType);
    }
};

int main() {
    // 1) 创建监听器
    MyLidarListener myListener;

    // 2) 创建解包器实例
    LIDARSampleDataUnpacker* unpacker = LIDARSampleDataUnpacker::CreateInstance(myListener);
    if (!unpacker) {
        fprintf(stderr, "创建激光雷达样本数据解包器失败！\n");
        return -1;
    }

    // 3) 启用解包器
    unpacker->enable();

    // --- 模拟原始数据输入 ---
    printf("模拟原始数据输入...\n");

    rplidar_response_measurement_node_hq_t simulated_hq_node{};
    simulated_hq_node.quality = 0xFF;  // 最大质量
    simulated_hq_node.angle_z_q14 = static_cast<_u32>(0 * 16384);  // 0 度
    simulated_hq_node.distance_q2 = 1000 * 4;  // 1000 mm
    simulated_hq_node.flag = RPLIDAR_RESP_MEASUREMENT_HQ_SYNCBIT;

    _u8 ansType = 0x2A;  // HQ 节点答案类型
    const void* raw_data_ptr = &simulated_hq_node;
    size_t raw_data_size = sizeof(rplidar_response_measurement_node_hq_t);

    // 首个节点
    unpacker->onSampleData(ansType, raw_data_ptr, raw_data_size);

    // 第二个节点：30 度，500 mm
    simulated_hq_node.quality = 0xC0;
    simulated_hq_node.angle_z_q14 =
        static_cast<_u32>(30 * 16384);  // 30 度
    simulated_hq_node.distance_q2 = 500 * 4;
    simulated_hq_node.flag = 0;  // 无同步位
    unpacker->onSampleData(ansType, raw_data_ptr, raw_data_size);

    // 5) 关闭并释放
    unpacker->disable();
    LIDARSampleDataUnpacker::ReleaseInstance(unpacker);

    return 0;
}


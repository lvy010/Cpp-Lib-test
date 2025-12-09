// Minimal demo implementation of the RPLIDAR data unpacker interfaces.
#pragma once

#include "../sl_lidar_driver.h"
#include <chrono>
#include <cstdio>
#include <memory>

namespace sl {
namespace rplidar {
namespace dataunpacker {

class LIDARSampleDataListener {
public:
    virtual ~LIDARSampleDataListener() = default;

    virtual void onHQNodeDecoded(_u64 timestamp_uS,
                                 const rplidar_response_measurement_node_hq_t* node) = 0;
    virtual void onHQNodeScanResetReq() {}
    virtual void onCustomSampleDataDecoded(_u8 /*ansType*/, _u32 /*customCode*/,
                                           const void* /*data*/, size_t /*size*/) {}
    virtual void onDecodingError(int /*errMsg*/, _u8 /*ansType*/,
                                 const void* /*payload*/, size_t /*size*/) {}
};

class LIDARSampleDataUnpacker {
public:
    static LIDARSampleDataUnpacker* CreateInstance(LIDARSampleDataListener& listener) {
        return new LIDARSampleDataUnpacker(listener);
    }

    static void ReleaseInstance(LIDARSampleDataUnpacker* inst) {
        delete inst;
    }

    void enable() { enabled_ = true; }
    void disable() { enabled_ = false; }

    // Feed raw sample data into the unpacker.
    void onSampleData(_u8 ansType, const void* payload, size_t size) {
        if (!enabled_) return;

        // Expect HQ nodes only (ansType 0x2A) with correct payload size.
        if (ansType != 0x2A ||
            size != sizeof(rplidar_response_measurement_node_hq_t) ||
            payload == nullptr) {
            listener_.onDecodingError(-1, ansType, payload, size);
            return;
        }

        const auto* node =
            static_cast<const rplidar_response_measurement_node_hq_t*>(payload);
        const auto ts = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::steady_clock::now().time_since_epoch())
                            .count();

        // Notify listener.
        listener_.onHQNodeDecoded(static_cast<_u64>(ts), node);

        // If sync bit set, signal scan reset.
        if (node->flag & RPLIDAR_RESP_MEASUREMENT_HQ_SYNCBIT) {
            listener_.onHQNodeScanResetReq();
        }
    }

private:
    explicit LIDARSampleDataUnpacker(LIDARSampleDataListener& listener)
        : listener_(listener) {}

    LIDARSampleDataListener& listener_;
    bool enabled_{false};
};

}  // namespace dataunpacker
}  // namespace rplidar
}  // namespace sl



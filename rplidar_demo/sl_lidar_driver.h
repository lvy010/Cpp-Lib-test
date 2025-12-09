// Minimal stub of sl_lidar_driver.h for demo purposes.
#pragma once

#include <cstdint>

using _u8 = uint8_t;
using _u16 = uint16_t;
using _u32 = uint32_t;
using _u64 = uint64_t;

// Sync flag for HQ nodes.
constexpr _u8 RPLIDAR_RESP_MEASUREMENT_HQ_SYNCBIT = 0x1;

// Simplified HQ measurement node structure.
struct rplidar_response_measurement_node_hq_t {
    _u32 angle_z_q14{};    // Angle in Q14 format (kept 32-bit for demo to avoid overflow).
    _u32 distance_q2{};    // Distance in Q2 format.
    _u8  quality{};        // Quality indicator.
    _u8  flag{};           // Flags (e.g., sync bit).
};


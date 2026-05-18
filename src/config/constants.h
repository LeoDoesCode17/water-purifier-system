#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H
#include <stdint.h> 
namespace constant {
    constexpr long BAUD_RATE = 115200;
    constexpr uint8_t ADS1115_ADDRESS_1 = 0x48;
    constexpr uint8_t ADS1115_ADDRESS_2 = 0x49;
    constexpr uint8_t CH_TDS =2;
    constexpr uint8_t CH_TURBIDITY = 0;
    constexpr uint8_t CH_PH = 1;
    constexpr float PH_NEUTRAL_VOLTAGE = 2.5f;
    constexpr float PH_SLOPE = 0.1776f;
    constexpr float ADS_BIT_MV = 0.125f; 
}
#endif
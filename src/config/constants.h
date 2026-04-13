#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H
#include <stdint.h> 
namespace constant {
    constexpr long BAUD_RATE = 115200;
    constexpr uint8_t ADS1115_ADDRESS_1 = 0x48;
    constexpr uint8_t ADS1115_ADDRESS_2 = 0x49;
}
#endif
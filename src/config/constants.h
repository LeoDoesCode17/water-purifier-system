#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H
#include <stdint.h>
namespace constant
{
    constexpr long BAUD_RATE = 115200;
    constexpr uint8_t ADS1115_ADDRESS_1 = 0x48;
    constexpr uint8_t ADS1115_ADDRESS_2 = 0x49;
    constexpr uint8_t CH_TDS = 2;
    constexpr uint8_t CH_TURBIDITY = 0;
    constexpr uint8_t CH_PH = 1;
    constexpr float PH_NEUTRAL_VOLTAGE = 2.5f;
    constexpr float ADS_BIT_MV = 0.125f;
    constexpr float MAX_MIXING_TANK_LEVEL = 7.0;
    constexpr float MAX_SETTLING_TANK_LEVEL = 9.0;
    constexpr unsigned long SENSOR_SAMPLE_COUNT = 200;
    constexpr unsigned long MQTT_RECONNECT_INTERVAL = 5000;
    constexpr float PH_SLOPE = -8.113910;
    constexpr float PH_INTERCEPT = 32.498114 + 0.6;
    constexpr float VOLTAGE_DIVIDER_R1 = 10000.0;
    constexpr float VOLTAGE_DIVIDER_R2 = 20000.0;
    constexpr float TURBIDITY_V_REF = 3.33;
    static const char *ACTUATOR_NAMES[] = {
        "servo_valve", // 0
        "gear_motor",  // 1
        "pump_1",      // 2
        "pump_2"       // 3
    };
    static const int ACTUATOR_COUNT = 4;
    constexpr float WATER_MASS_GR_SLOPE = -321.1096;
    constexpr float WATER_MASS_GR_INTERCEPT = 9818.1436;
    constexpr float WATER_DENSITY_GR_PER_ML = 1;
    constexpr float LITER_TO_ML = 1000.0;
    constexpr unsigned long N_SAMPLES = 10;
    constexpr unsigned long SAMPLE_DELAY_MS = 40; // ~25Hz, gives ADS1115 time to settle
}
#endif
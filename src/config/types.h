#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H
#include <stdint.h>

typedef struct {
    float ntu;
    float ph;
    float ppm;
    float level;
    float temperature;
    float volume_liter;
} WaterQuality;

using mqtt_callback = void(*)(char*, uint8_t*, unsigned int);

enum tank_type {
    TANK_RAW, 
    TANK_SETTLING
};

enum sensor_type {
    TDS, 
    TURBIDITY, 
    PH, 
    TEMPERATURE, 
    WATER_VOLUME
};

enum state_machine {
    IDLE, 
    FILLING_MIXING_TANK, 
    RAW_SAMPLING, 
    MEASURE_MORINGA, 
    DOSING, 
    MIXING, 
    SETTLING, 
    FILLING_SETTLING_TANK,
    COMPLETED, 
    FAILED
};

enum device_status {
    ONLINE, 
    OFFLINE
};
#endif
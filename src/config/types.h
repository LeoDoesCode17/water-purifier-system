#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H
typedef struct {
    float ntu;
    float ph;
    float ppm;
    float level;
    float temperature;
} WaterQuality;

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
    FILLING_TANK, 
    RAW_SAMPLING, 
    MEASURE_MORINGA, 
    DOSING, 
    MIXING, 
    SETTLING, 
    COMPLETED, 
    FAILED
};

enum device_status {
    ONLINE, 
    OFFLINE
};
#endif